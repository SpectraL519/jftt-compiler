#include "compiler.hpp"

#include <iostream>



namespace jftt {

void compiler::finish_code_generating() {
    this->_asm_builder.stop_building();
}

const std::vector<std::string>& compiler::asm_code() const {
    return this->_asm_builder.code();
}

void compiler::set_line_no(const std::size_t line_no) {
    this->_line_no = line_no;
}

void compiler::declare_variable(const std::string& name) {
    static constexpr auto discriminator{identifier_discriminator::variable};
    this->assert_no_identifier_redeclaration(name);
    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(name));

    const auto& identifier{this->_identifier_manager.get<discriminator>(name)};
    identifier->set_address(this->_memory_manager.allocate(identifier->size()));
}

void compiler::declare_vararray(
    const std::string& name,
    const architecture::memory_size_type size
) {
    static constexpr auto discriminator{identifier_discriminator::vararray};
    this->assert_no_identifier_redeclaration(name);
    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(name, size));

    const auto& identifier{this->_identifier_manager.get<discriminator>(name)};
    identifier->set_address(this->_memory_manager.allocate(identifier->size()));
}

void compiler::declare_procedure(const std::string& name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_no_identifier_redeclaration(name);
    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(name));
}

const std::shared_ptr<identifier::abstract_identifier>& compiler::get_identifier(
    const std::string& name
) {
    return this->_identifier_manager.get(name);
}

void compiler::initialize_lvalue_identifier(const std::string& name) {
    this->assert_identifier_defined(name);
    this->_identifier_manager.initialize_lvalue_identifier(name);
}

void compiler::scan(identifier::abstract_identifier* identifier) {
    switch (identifier->discriminator()) {
    case identifier_discriminator::variable: {
        this->initialize_lvalue_identifier(identifier->name());
        this->_asm_builder.read_lvalue(
            identifier::shared_ptr_cast<identifier_discriminator::variable>(identifier));
        break;
    }

    case identifier_discriminator::vararray: {
        this->initialize_lvalue_identifier(identifier->name());
        this->_asm_builder.read_lvalue(
            identifier::shared_ptr_cast<identifier_discriminator::vararray>(identifier));
        break;
    }

    default:
        std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
                  << "\tCannot read identifier: " << identifier->name() << std::endl;
        std::exit(1);
    }
}

void compiler::print(identifier::abstract_identifier* identifier) {
    this->_asm_builder.write_identifier(identifier::shared_ptr_cast(identifier));
}

void compiler::acquire_accumulator() {
    this->_memory_manager.get_accumulator().acquire();
}

void compiler::release_accumulator() {
    this->_memory_manager.get_accumulator().release();
}

void compiler::return_value(identifier::abstract_identifier* identifier) {
    // stores identifier's value in acc
    this->assert_lvalue_initialized(identifier->name(), identifier->discriminator());
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(identifier), this->_memory_manager.get_accumulator());
}

void compiler::assign_value_to(identifier::abstract_identifier* identifier) {
    // assigns value from acc to an lvalue identifier
    this->initialize_lvalue_identifier(identifier->name());
    auto lvalue{
        identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier)};

    auto& lvalue_address_register{this->_memory_manager.acquire_free_register()};

    auto& tmp_register{this->_asm_builder.move_acc_content_to_tmp_register()};
    this->_asm_builder.initialize_addres_in_register(lvalue, lvalue_address_register);
    this->_asm_builder.move_tmp_register_content_to_acc(tmp_register);
    this->_asm_builder.add_instruction(assembly::instructions::store(lvalue_address_register));

    lvalue_address_register.release();
}

void compiler::add_condition(
    const condition_discriminator discriminator,
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b
) {
    // TODO: load values in the asm_builder functions to avoid excessive loading

    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(a), a_register);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(b), b_register);

    switch (discriminator) {
    case condition_discriminator::eq:
        this->_condition_manager.add_branch(
            this->_asm_builder.equal_condition(a_register, b_register));
        break;

    case condition_discriminator::neq:
        this->_condition_manager.add_branch(
            this->_asm_builder.not_equal_condition(a_register, b_register));
        break;

    case condition_discriminator::le:
        this->_condition_manager.add_branch(
            this->_asm_builder.less_condition(a_register, b_register));
        break;

    case condition_discriminator::leq:
        this->_condition_manager.add_branch(
            this->_asm_builder.less_equal_condition(a_register, b_register));
        break;

    case condition_discriminator::ge:
        this->_condition_manager.add_branch(
            this->_asm_builder.greater_condition(a_register, b_register));
        break;

    case condition_discriminator::geq:
        this->_condition_manager.add_branch(
            this->_asm_builder.greater_equal_condition(a_register, b_register));
        break;
    }

    a_register.release();
    b_register.release();
}

void compiler::end_latest_condition_without_else() {
    const auto latest_branch{this->_condition_manager.extract_branch()};
    this->_asm_builder.new_condition_branch_jump_point(latest_branch);
}

void compiler::end_latest_condition_with_else() {
    // add a jump point to the end of the if_else branch
    const auto cond_end_branch{condition::branch{
        this->_asm_builder.new_condition_jump_label("cond_end")}};
    this->_asm_builder.set_condition_branch_jump_point(cond_end_branch);

    // add a jump point skipping the else branch if condition is evaluated to true
    this->_asm_builder.new_condition_branch_jump_point(this->_condition_manager.extract_branch());

    // add the cond_end_branch to the stack
    this->_condition_manager.add_branch(cond_end_branch);
}


void compiler::add_loop(const loop_discriminator discriminator) {
    switch (discriminator) {
    case loop_discriminator::while_do: {
        const std::string begin_label{this->_asm_builder.new_condition_jump_label("while_loop_begin")};
        // end label will be defined as the condition end label
        std::shared_ptr<loop::abstract_loop> loop{
            std::make_shared<loop::while_loop>(begin_label, std::string{})};
        this->_loop_manager.add_loop(loop);
        this->_asm_builder.start_loop(loop);
        break;
    }

    case loop_discriminator::repeat_until: {
        const std::string begin_label{
            this->_asm_builder.new_condition_jump_label("repeat_until_loop_begin")};
        // end label will be defined as the condition end label
        std::shared_ptr<loop::abstract_loop> loop{
            std::make_shared<loop::repeat_until_loop>(begin_label, std::string{})};
        this->_loop_manager.add_loop(loop);
        this->_asm_builder.start_loop(loop);
        break;
    }
    }
}

void compiler::set_latest_loop_end_label() {
    const auto loop_branch{this->_condition_manager.extract_branch()};
    this->_loop_manager.get_loop()->set_end_label(loop_branch.false_eval_label);
}

void compiler::end_loop(const loop_discriminator discriminator) {
    switch (discriminator) {
    case loop_discriminator::while_do: {
        const auto loop{this->_loop_manager.extract_loop()};
        this->_asm_builder.end_while_loop(loop);
        break;
    }

    case loop_discriminator::repeat_until: {
        const auto loop{this->_loop_manager.extract_loop()};
        this->_asm_builder.end_repeat_until_loop(loop);
        break;
    }
    }
}

// TODO: arithmetic_operation(discriminator, a, b)
void compiler::add(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    this->assert_lvalue_initialized(a->name(), a->discriminator());
    this->assert_lvalue_initialized(b->name(), b->discriminator());

    auto& a_register{this->_memory_manager.acquire_free_register()};

    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(a), a_register);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(b), this->_memory_manager.get_accumulator());

    this->_asm_builder.add_instruction(assembly::instructions::add(a_register));
    a_register.release();
}

// TODO: arithmetic_operation(discriminator, a, b)
void compiler::subtract(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    this->assert_lvalue_initialized(a->name(), a->discriminator());
    this->assert_lvalue_initialized(b->name(), b->discriminator());

    this->_asm_builder.subtract(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

// TODO: arithmetic_operation(discriminator, a, b)
void compiler::multiply(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    this->assert_lvalue_initialized(a->name(), a->discriminator());
    this->assert_lvalue_initialized(b->name(), b->discriminator());

    this->_asm_builder.multiply(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

// TODO: arithmetic_operation(discriminator, a, b)
void compiler::divide(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    this->assert_lvalue_initialized(a->name(), a->discriminator());
    this->assert_lvalue_initialized(b->name(), b->discriminator());

    this->_asm_builder.divide(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

// TODO: arithmetic_operation(discriminator, a, b)
void compiler::modulo(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    this->assert_lvalue_initialized(a->name(), a->discriminator());
    this->assert_lvalue_initialized(b->name(), b->discriminator());

    this->_asm_builder.modulo(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

void compiler::assert_no_identifier_redeclaration(
    const std::string& identifier_name
) const {
    if (!this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tIdentifier `" << identifier_name << "` already defined" << std::endl;
    std::exit(1);
}

void compiler::assert_identifier_defined(const std::string& identifier_name) const {
    if (this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

void compiler::assert_identifier_defined(
    const std::string& identifier_name, const identifier_discriminator discriminator
) const {
    if (this->_identifier_manager.has(identifier_name, discriminator))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tUndefined " << identifier::as_string(discriminator)
              << " identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

void compiler::assert_lvalue_initialized(
    const std::string& identifier_name,
    const identifier_discriminator discriminator
) const {
    // TODO: vararray initialization should be checked per index

    if (discriminator == identifier_discriminator::rvalue)
        return;

    this->assert_identifier_defined(identifier_name);

    const auto lvalue{
        this->_identifier_manager.get<identifier_discriminator::lvalue>(identifier_name)};
    if (lvalue->is_initialized())
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tUninitialized identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}


} // namespace jftt

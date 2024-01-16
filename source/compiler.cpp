#include "compiler.hpp"

#include <iostream>



namespace jftt {

void compiler::with_procedurers() {
    if (this->_program_begin_label)
        return;

    this->_program_begin_label.emplace(this->_asm_builder.new_jump_label("program_begin"));
    this->_asm_builder.set_jump_point(this->_program_begin_label.value());
}

void compiler::begin_program() {
    if (this->_program_begin_label)
        this->_asm_builder.insert_jump_point_label(this->_program_begin_label.value());
}

void compiler::finish_code_generating() {
    this->_asm_builder.stop_building();
}

const std::vector<std::string>& compiler::asm_code() const {
    return this->_asm_builder.code();
}

void compiler::set_line_no(const std::size_t line_no) {
    this->_line_no = line_no;
}

void compiler::declare_variable(
    const std::string& name, const std::optional<std::string>& procedure_name
) {
    static constexpr auto discriminator{identifier_discriminator::variable};
    this->assert_no_identifier_redeclaration(name, procedure_name);

    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        auto identifier{std::make_shared<identifier::type<discriminator>>(name)};
        identifier->set_address(this->_memory_manager.allocate(identifier->size()));
        procedure->declare_local_identifier(std::move(identifier));
        return;
    }

    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(name));

    const auto identifier{this->_identifier_manager.get<discriminator>(name)};
    identifier->set_address(this->_memory_manager.allocate(identifier->size()));
}

void compiler::declare_vararray(
    const std::string& name,
    const architecture::memory_size_type size,
    const std::optional<std::string>& procedure_name
) {
    static constexpr auto discriminator{identifier_discriminator::vararray};
    this->assert_no_identifier_redeclaration(name, procedure_name);

    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        auto identifier{std::make_shared<identifier::type<discriminator>>(name, size)};
        identifier->set_address(this->_memory_manager.allocate(identifier->size()));
        procedure->declare_local_identifier(std::move(identifier));

        return;
    }

    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(name, size));

    const auto& identifier{this->_identifier_manager.get<discriminator>(name)};
    identifier->set_address(this->_memory_manager.allocate(identifier->size()));
}

void compiler::declare_procedure(const std::string& name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_no_identifier_redeclaration(name);

    this->_identifier_manager.add<discriminator>(
        std::make_shared<identifier::type<discriminator>>(
            name, this->_memory_manager.allocate(1u)));
}

void compiler::declare_procedure_parameter(
    const std::string& procedure_name,
    const identifier_discriminator param_discriminator,
    const std::string& local_name
) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_identifier_defined(procedure_name, discriminator);

    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    auto reference_variant{procedure->declare_parameter(local_name, param_discriminator)};
    if (std::holds_alternative<std::string>(reference_variant))
        this->throw_error(std::get<std::string>(reference_variant));

    // allocate memory for parameter reference addres
    auto reference{std::get<std::shared_ptr<identifier::reference>>(reference_variant)};
    reference->set_address(this->_memory_manager.allocate(reference->size()));
}

void compiler::begin_procedure_implementation(const std::string& procedure_name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_identifier_defined(procedure_name, discriminator);
    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    const std::string procedure_begin_label{
        this->_asm_builder.new_jump_label(procedure_name + "_begin")};
    procedure->set_begin_label(procedure_begin_label);
    this->_asm_builder.insert_jump_point_label(procedure_begin_label);

    // store the procedure return point in the specified memory address
    auto& accumulator{this->_memory_manager.get_accumulator()};
    auto& return_point_address_register{this->_memory_manager.acquire_free_register()};

    this->_asm_builder.initialize_value_in_register(
        procedure->return_point_address(), return_point_address_register);

    this->_asm_builder.add_instruction(assembly::instructions::inc(accumulator));
    this->_asm_builder.add_instruction(assembly::instructions::inc(accumulator));
    this->_asm_builder.add_instruction(
        assembly::instructions::store(return_point_address_register));

    return_point_address_register.release();
}

void compiler::pass_procedure_parameter(
    const std::string& procedure_name,
    identifier::abstract_identifier* identifier,
    const std::optional<std::string>& current_procedure
) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    auto lvalue{identifier::shared_ptr_cast<identifier_discriminator::lvalue>(
        this->get_identifier(identifier->name(), current_procedure))};

    this->assert_identifier_defined(procedure_name, discriminator);
    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    std::shared_ptr<identifier::reference> parameter{procedure->get_next_parameter()};
    if (!parameter)
        this->throw_error("Too many parameters passed for procedure: " + procedure->name());

    bool valid_lvalue_discriminator{true};
    if (lvalue->discriminator() == identifier_discriminator::reference) {
        auto reference{identifier::shared_ptr_cast<identifier_discriminator::reference>(lvalue)};
        if (reference->reference_discriminator() != parameter->reference_discriminator())
            valid_lvalue_discriminator = false;
    }
    else {
        if (parameter->initializes_underlying_resource())
            lvalue->initialize();
        else if (!lvalue->is_initialized())
            this->throw_error(
                "Passing an unitialized identifier `" + lvalue->name() +
                "` to a procedure: " + procedure->name());

        if (lvalue->discriminator() != parameter->reference_discriminator())
            valid_lvalue_discriminator = false;
    }

    if (!valid_lvalue_discriminator)
        this->throw_error(
            "Invalid parameter passed for procedure: " + procedure->name() +
            "\n\texpected - " + identifier::as_string(parameter->reference_discriminator()) +
            "; got - " + identifier::as_string(lvalue->discriminator()));

    architecture::vm_register* tmp_register{nullptr};
    auto& accumulator{this->_memory_manager.get_accumulator()};
    if (!accumulator.is_free())
        tmp_register = &this->_asm_builder.move_acc_content_to_tmp_register();

    auto& reference_address_register{this->_memory_manager.acquire_free_register()};
    this->_asm_builder.initialize_value_in_register(parameter->address(), reference_address_register);
    this->_asm_builder.initialize_address_in_register(lvalue, accumulator);
    this->_asm_builder.add_instruction(assembly::instructions::store(reference_address_register));

    if (tmp_register)
        this->_asm_builder.move_tmp_register_content_to_acc(*tmp_register);
    reference_address_register.release();
}

void compiler::end_procedure_call_args_declaration(const std::string& procedure_name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_identifier_defined(procedure_name, discriminator);
    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    auto error_opt{procedure->finish_parameter_passing()};
    if (error_opt)
        this->throw_error(error_opt.value());
}

void compiler::return_from_procedure(const std::string& procedure_name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_identifier_defined(procedure_name, discriminator);
    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    auto& accumulator{this->_memory_manager.get_accumulator()};

    // initialize the return point value in accumulator
    this->_asm_builder.initialize_value_in_register(
        procedure->return_point_address(), accumulator);
    this->_asm_builder.add_instruction(assembly::instructions::load(accumulator));

    // jump to the return point currently stored in the accumulator
    this->_asm_builder.add_instruction(assembly::instructions::jumpr(accumulator));
}

void compiler::call_procedure(const std::string& procedure_name) {
    static constexpr auto discriminator{identifier_discriminator::procedure};
    this->assert_identifier_defined(procedure_name, discriminator);
    auto procedure{
        identifier::shared_ptr_cast<discriminator>(this->get_identifier(procedure_name))};

    // initialize the current asm line in accumulator
    this->_asm_builder.add_instruction(
        assembly::instructions::strk(this->_memory_manager.get_accumulator()));

    // jump to the begin of the procedure
    this->_asm_builder.set_jump_point(procedure->begin_label());
}

std::shared_ptr<identifier::abstract_identifier> compiler::get_identifier(
    const std::string& name, const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        auto identifier{procedure->get_identifier(name)};
        return identifier;
    }

    return this->_identifier_manager.get(name);
}

void compiler::initialize_lvalue_identifier(
    const std::string& name, const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        auto lvalue{procedure->get_identifier(name)};
        if (lvalue->discriminator() == identifier_discriminator::reference) {
            auto reference{identifier::shared_ptr_cast<identifier_discriminator::reference>(lvalue)};
            reference->initialize_resource();
        }
        else {
            lvalue->initialize();
        }

        return;
    }

    this->assert_identifier_defined(name);
    this->_identifier_manager.initialize_lvalue_identifier(name);
}

void compiler::scan(
    identifier::abstract_identifier* identifier,
    const std::optional<std::string>& procedure_name
) {
    switch (identifier->discriminator()) {
    case identifier_discriminator::variable: {
        this->initialize_lvalue_identifier(identifier->name(), procedure_name);
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
        this->throw_error("Cannot read identifier: " + identifier->name());
    }
}

void compiler::print(
    identifier::abstract_identifier* identifier,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(
        identifier->name(), identifier->discriminator(), procedure_name);
    this->_asm_builder.write_identifier(identifier::shared_ptr_cast(identifier));
}

void compiler::acquire_accumulator() {
    this->_memory_manager.get_accumulator().acquire();
}

void compiler::release_accumulator() {
    this->_memory_manager.get_accumulator().release();
}

void compiler::return_value(
    identifier::abstract_identifier* identifier,
    const std::optional<std::string>& procedure_name
) {
    // stores identifier's value in acc
    this->assert_lvalue_initialized(
        identifier->name(), identifier->discriminator(), procedure_name);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(identifier), this->_memory_manager.get_accumulator());
}

void compiler::assign_value_to(
    identifier::abstract_identifier* identifier,
    const std::optional<std::string>& procedure_name
) {
    // assigns value from acc to an lvalue identifier
    this->initialize_lvalue_identifier(identifier->name(), procedure_name);
    auto lvalue{
        identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier)};

    auto& lvalue_address_register{this->_memory_manager.acquire_free_register()};

    auto& tmp_register{this->_asm_builder.move_acc_content_to_tmp_register()};
    this->_asm_builder.initialize_address_in_register(lvalue, lvalue_address_register);
    this->_asm_builder.move_tmp_register_content_to_acc(tmp_register);
    this->_asm_builder.add_instruction(assembly::instructions::store(lvalue_address_register));

    lvalue_address_register.release();
}

void compiler::add_condition(
    const condition_discriminator discriminator,
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    switch (discriminator) {
    case condition_discriminator::eq:
        this->_condition_manager.add_branch(this->_asm_builder.equal_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;

    case condition_discriminator::neq:
        this->_condition_manager.add_branch(this->_asm_builder.not_equal_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;

    case condition_discriminator::le:
        this->_condition_manager.add_branch(this->_asm_builder.less_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;

    case condition_discriminator::leq:
        this->_condition_manager.add_branch(this->_asm_builder.less_equal_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;

    case condition_discriminator::ge:
        this->_condition_manager.add_branch(this->_asm_builder.greater_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;

    case condition_discriminator::geq:
        this->_condition_manager.add_branch(this->_asm_builder.greater_equal_condition(
            identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b)));
        break;
    }
}

void compiler::end_latest_condition_without_else() {
    const auto latest_branch{this->_condition_manager.extract_branch()};
    this->_asm_builder.insert_jump_point_label(latest_branch.false_eval_label);
}

void compiler::end_latest_condition_with_else() {
    // add a jump point to the end of the if_else branch
    const auto cond_end_branch{condition::branch{
        this->_asm_builder.new_jump_label("cond_end")}};
    this->_asm_builder.set_jump_point(cond_end_branch.false_eval_label);

    // add a jump point skipping the else branch if condition is evaluated to true
    this->_asm_builder.insert_jump_point_label(
        this->_condition_manager.extract_branch().false_eval_label);

    // add the cond_end_branch to the stack
    this->_condition_manager.add_branch(cond_end_branch);
}


void compiler::add_loop(const loop_discriminator discriminator) {
    switch (discriminator) {
    case loop_discriminator::while_do: {
        const std::string begin_label{this->_asm_builder.new_jump_label("while_loop_begin")};
        // end label will be defined as the condition end label
        std::shared_ptr<loop::abstract_loop> loop{
            std::make_shared<loop::while_loop>(begin_label, std::string{})};
        this->_loop_manager.add_loop(loop);
        this->_asm_builder.start_loop(loop);
        break;
    }

    case loop_discriminator::repeat_until: {
        const std::string begin_label{
            this->_asm_builder.new_jump_label("repeat_until_loop_begin")};
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

void compiler::add(
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    auto& a_register{this->_memory_manager.acquire_free_register()};

    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(a), a_register);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(b), this->_memory_manager.get_accumulator());

    this->_asm_builder.add_instruction(assembly::instructions::add(a_register));
    a_register.release();
}

void compiler::subtract(
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    this->_asm_builder.subtract(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

void compiler::multiply(
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    this->_asm_builder.multiply(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

void compiler::divide(
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    this->_asm_builder.divide(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

void compiler::modulo(
    identifier::abstract_identifier* a,
    identifier::abstract_identifier* b,
    const std::optional<std::string>& procedure_name
) {
    this->assert_lvalue_initialized(a->name(), a->discriminator(), procedure_name);
    this->assert_lvalue_initialized(b->name(), b->discriminator(), procedure_name);

    this->_asm_builder.modulo(
        identifier::shared_ptr_cast(a), identifier::shared_ptr_cast(b));
}

void compiler::assert_no_identifier_redeclaration(
    const std::string& identifier_name,
    const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        if (!procedure->has_identifier(identifier_name))
            return;

        this->throw_error(
            "Identifier `" + identifier_name +
            "` already defined in procedure: " + procedure_name.value());
    }

    if (!this->_identifier_manager.has(identifier_name))
        return;

    this->throw_error("Identifier `" + identifier_name + "` already defined");
}

void compiler::assert_identifier_defined(
    const std::string& identifier_name,
    const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        if (procedure->has_identifier(identifier_name))
            return;

        this->throw_error(
            "Undefined identifier `" + identifier_name + "` in procedure: " + procedure_name.value());
    }

    if (this->_identifier_manager.has(identifier_name))
        return;

    this->throw_error("Undefined identifier `" + identifier_name + "`");
}

void compiler::assert_identifier_defined(
    const std::string& identifier_name,
    const identifier_discriminator discriminator,
    const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        if (procedure->has_identifier(identifier_name, discriminator))
            return;

        this->throw_error(
            "Undefined " + identifier::as_string(discriminator) + " identifier `" +
            identifier_name + "` in procedure: " + procedure_name.value());
    }

    if (this->_identifier_manager.has(identifier_name, discriminator))
        return;

    this->throw_error(
        "Undefined " + identifier::as_string(discriminator) + " identifier `" + identifier_name + "`");
}

void compiler::assert_identifier_defined(
    const std::string& identifier_name,
    const std::vector<identifier_discriminator>& discriminator_list,
    const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        for (const auto discriminator : discriminator_list)
            if (procedure->has_identifier(identifier_name, discriminator))
                return;

        this->throw_error(
            "Uninitialized identifier `" + identifier_name +
            "` in procedure: " + procedure_name.value());
    }

    for (const auto discriminator : discriminator_list)
        if (this->_identifier_manager.has(identifier_name, discriminator))
            return;

    this->throw_error("Uninitialized identifier `" + identifier_name + "`");
}

void compiler::assert_lvalue_initialized(
    const std::string& identifier_name,
    const identifier_discriminator discriminator,
    const std::optional<std::string>& procedure_name
) {
    if (!identifier::is_lvalue(discriminator))
        return;

    if (this->_condition_manager.has_branches()) {
        this->_warn_uninitialized_identifier_condition(identifier_name, procedure_name);
        return;
    }

    if (this->_loop_manager.has_loops() &&
        this->_loop_manager.get_loop()->discriminator() == loop_discriminator::while_do
    ) {
        this->_warn_uninitialized_identifier_condition(identifier_name, procedure_name);
        return;
    }

    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        this->assert_identifier_defined(identifier_name, procedure_name);
        const auto local_identifier{procedure->get_identifier(identifier_name)};
        if (local_identifier->is_initialized())
            return;

        this->throw_error(
            "Uninitialized identifier `" + identifier_name +
            "` in procedure: " + procedure_name.value());
    }

    this->assert_identifier_defined(identifier_name);
    const auto lvalue{
        this->_identifier_manager.get<identifier_discriminator::lvalue>(identifier_name)};

    if (lvalue->is_initialized())
        return;

    this->throw_error("Uninitialized identifier `" + identifier_name + "`");
}

void compiler::assert_index_in_range(
    const std::string& vararray_name,
    const architecture::value_type index,
    const std::optional<std::string>& procedure_name
) {
    this->assert_identifier_defined(
        vararray_name, identifier_discriminator::vararray, procedure_name);

    auto identifier{this->get_identifier(vararray_name, procedure_name)};
    if (identifier->discriminator() == identifier_discriminator::reference)
        return;

    auto vararray{
        identifier::shared_ptr_cast<identifier_discriminator::vararray>(identifier)};
    if (index < vararray->size())
        return;

    this->throw_error(
        "Index " + std::to_string(index) + " out of range for vararray: `" +
        vararray->name() + "`");
}

void compiler::_warn_uninitialized_identifier_condition(
    const std::string& identifier_name,
    const std::optional<std::string>& procedure_name
) {
    if (procedure_name) {
        this->assert_identifier_defined(
            procedure_name.value(), identifier_discriminator::procedure);
        auto procedure{identifier::shared_ptr_cast<identifier_discriminator::procedure>(
            this->get_identifier(procedure_name.value()))};

        this->assert_identifier_defined(identifier_name, procedure_name);
        const auto local_lvalue{procedure->get_identifier(identifier_name)};
        if (!local_lvalue->is_initialized())
            std::cerr << "[WARNING] In line: " << this->_line_no << std::endl
                      << "\tUsing an uninitialized identifier `" << identifier_name
                      << "` in a condition block in procedure: " << procedure_name.value() << std::endl;

        return;
    }

    this->assert_identifier_defined(identifier_name);
    const auto lvalue{
        this->_identifier_manager.get<identifier_discriminator::lvalue>(identifier_name)};

    if (!lvalue->is_initialized())
        std::cerr << "[WARNING] In line: " << this->_line_no << std::endl
                  << "\tUsing an uninitialized identifier `" << identifier_name
                  << "` in a condition block" << std::endl;
}

void compiler::throw_error(const std::string& msg) const {
    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\t" << msg << std::endl;
    std::exit(1);
}

} // namespace jftt

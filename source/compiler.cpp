#include "compiler.hpp"

#include "identifier_manager.hpp"
#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"

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
        std::make_unique<identifier::type<discriminator>>(name));

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
        std::make_unique<identifier::type<discriminator>>(name, size));

    const auto& identifier{this->_identifier_manager.get<discriminator>(name)};
    identifier->set_address(this->_memory_manager.allocate(identifier->size()));
}

// void compiler::declare_procedure(const std::string& name) {
//     static constexpr auto discriminator{identifier_discriminator::procedure};
//     this->assert_no_identifier_redeclaration(name);
//     this->_identifier_manager.add<discriminator>(
//         std::make_unique<identifier::type<discriminator>>(name));
// }

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
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(identifier), this->_memory_manager.get_accumulator());
}

void compiler::assign_value_to(identifier::abstract_identifier* identifier) {
    // assigns value from acc to an lvalue identifier
    this->initialize_lvalue_identifier(identifier->name());
    auto lvalue{
        identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier)};

    auto& lvalue_address_register{this->_memory_manager.get_free_register()};
    lvalue_address_register.acquire();

    auto& tmp_register{this->_asm_builder._move_acc_content_to_tmp_register()};
    this->_asm_builder.initialize_addres_in_register(lvalue, lvalue_address_register);
    this->_asm_builder._move_tmp_register_content_to_acc(tmp_register);
    this->_asm_builder.add_instruction(assembly::instructions::store(lvalue_address_register));

    lvalue_address_register.release();
}

void compiler::add(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    auto& a_register{this->_memory_manager.get_free_register()};
    a_register.acquire();

    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(a), a_register);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(b), this->_memory_manager.get_accumulator());

    this->_asm_builder.add_instruction(assembly::instructions::add(a_register));
    a_register.release();
}

void compiler::subtract(identifier::abstract_identifier* a, identifier::abstract_identifier* b) {
    auto& b_register{this->_memory_manager.get_free_register()};
    b_register.acquire();

    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(b), b_register);
    this->_asm_builder.initialize_identifier_value_in_register(
        identifier::shared_ptr_cast(a), this->_memory_manager.get_accumulator());

    this->_asm_builder.add_instruction(assembly::instructions::sub(b_register));
    b_register.release();
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
              << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

void compiler::assert_lvalue_initialized(const std::string& lvalue_name) const {
    this->assert_identifier_defined(lvalue_name);

    const auto lvalue{
        this->_identifier_manager.get<identifier_discriminator::lvalue>(lvalue_name)};
    if (lvalue->is_initialized())
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tUninitialized identifier `" << lvalue_name << '`' << std::endl;
    std::exit(1);
}


} // namespace jftt

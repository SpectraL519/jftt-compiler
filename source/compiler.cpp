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
}

void compiler::declare_vararray(
    const std::string& name,
    const architecture::memory_size_type size
) {
    static constexpr auto discriminator{identifier_discriminator::vararray};
    this->assert_no_identifier_redeclaration(name);
    this->_identifier_manager.add<discriminator>(
        std::make_unique<identifier::type<discriminator>>(name, size));
}

// void compiler::declare_procedure(const std::string& name) {
//     static constexpr auto discriminator{identifier_discriminator::procedure};
//     this->assert_no_identifier_redeclaration(name);
//     this->_identifier_manager.add<discriminator>(
//         std::make_unique<identifier::type<discriminator>>(name));
// }

identifier::abstract_identifier* compiler::get_identifier(const std::string& name) {
    return this->_identifier_manager.has(name)
        ? this->_identifier_manager.get(name).get()
        : nullptr;
}

void compiler::scan(identifier::abstract_identifier* identifier) {
    switch (identifier->discriminator()) {
    case identifier_discriminator::variable: {
        auto variable{
            this->_identifier_manager.get<identifier_discriminator::variable>(identifier->name())};
        variable->initialize();
        this->_asm_builder.read_variable(variable);
        break;
    }

    case identifier_discriminator::vararray: {
        auto vararray{
            this->_identifier_manager.get<identifier_discriminator::vararray>(identifier->name())};
        vararray->initialize();
        this->_asm_builder.read_vararray_element(vararray);
        break;
    }

    default:
        std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
                  << "\tCannot read identifier: " << identifier->name() << std::endl;
        std::exit(1);
    }
}

void compiler::print(identifier::abstract_identifier* identifier) {
    switch (identifier->discriminator()) {
    case identifier_discriminator::rvalue:
        this->_asm_builder.write_value(
            identifier::raw_ptr_cast<identifier_discriminator::rvalue>(identifier)->value());
        break;

    case identifier_discriminator::variable:
        this->_asm_builder.write_variable(
            identifier::raw_ptr_cast<identifier_discriminator::variable>(identifier)->address());
        break;

    default:
        break; // TODO
    }
}

void compiler::assign_value_to_variable(
    const std::string& variable_name, architecture::vm_register& value_register
) {
    this->assert_identifier_defined(variable_name, identifier_discriminator::variable);

    // const auto& variable = this->_identifier_manager.get(variable_name);
    // TODO: this->_asm_builder.store(variable->address(), value_register);
}

void compiler::assign_value_to_vararrray_element(
    const std::string& vararray_name,
    const architecture::memory_size_type vararray_idx,
    architecture::vm_register& value_register
) {
    this->assert_identifier_defined(vararray_name, identifier_discriminator::vararray);

    // const auto& variable = this->_identifier_manager.get(vararray_name);
    // TODO: this->_asm_builder.store(variable->address() + varraray_idx, value_register);
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

    const auto& lvalue{identifier::shared_ptr_cast<identifier_discriminator::lvalue>(
                        this->_identifier_manager.get(lvalue_name))};
    if (lvalue->is_initialized())
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
              << "\tUninitialized identifier `" << lvalue_name << '`' << std::endl;
    std::exit(1);
}


} // namespace jftt

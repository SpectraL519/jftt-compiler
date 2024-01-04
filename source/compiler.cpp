#include "compiler.hpp"

#include "identifier_manager.hpp"
#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"

#include <iostream>



namespace jftt {

void compiler::read_input() {}

void compiler::declare_variable(const uint64_t line_no, const std::string& variable_name) {
    this->_assert_no_identifier_redeclaration(line_no, variable_name);
    this->_identifier_manager.add_variable(variable_name);
}

void compiler::declare_vararray(
    const uint64_t line_no,
    const std::string& vararray_name,
    const architecture::memory_size_type size
) {
    this->_assert_no_identifier_redeclaration(line_no, vararray_name);
    this->_identifier_manager.add_vararray(vararray_name, size);
}

void compiler::declare_procedure(const uint64_t line_no, const std::string& procedure_name) {
    this->_assert_no_identifier_redeclaration(line_no, procedure_name);
    this->_identifier_manager.add_procedure(procedure_name);
}

void compiler::set_identifier_used(const uint64_t line_no, const std::string& identifier_name) {
    this->_assert_identifier_defined(line_no, identifier_name);
    this->_identifier_manager.get(identifier_name)->set_used();
}

void compiler::assign_value_to_variable(
    const uint64_t line_no,
    const std::string& variable_name,
    architecture::vm_register& value_register
) {
    this->_assert_identifier_defined(line_no, variable_name, identifier_discriminator::variable);

    // const auto& variable = this->_identifier_manager.get(variable_name);
    // TODO: this->_asm_builder.store(variable->address(), value_register);
}

void compiler::assign_value_to_vararrray_element(
    const uint64_t line_no,
    const std::string& vararray_name,
    const architecture::memory_size_type vararray_idx,
    architecture::vm_register& value_register
) {
    this->_assert_identifier_defined(line_no, vararray_name, identifier_discriminator::vararray);

    // const auto& variable = this->_identifier_manager.get(vararray_name);
    // TODO: this->_asm_builder.store(variable->address() + varraray_idx, value_register);
}

void compiler::_assert_no_identifier_redeclaration(
    const uint64_t line_no, const std::string& identifier_name
) const {
    if (!this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << line_no << std::endl
                << "\tIdentifier `" << identifier_name << "` already defined" << std::endl;
    std::exit(1);
}

void compiler::_assert_identifier_defined(
    const uint64_t line_no,
    const std::string& identifier_name
) const {
    if (this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << line_no << std::endl
                << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

void compiler::_assert_identifier_defined(
    const uint64_t line_no,
    const std::string& identifier_name,
    const identifier_discriminator discriminator
) const {
    if (this->_identifier_manager.has(identifier_name, discriminator))
        return;

    std::cerr << "[ERROR] In line: " << line_no << std::endl
                << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

} // namespace jftt

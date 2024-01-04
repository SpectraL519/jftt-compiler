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

void compiler::declare_variable(const std::string& variable_name) {
    this->_assert_no_identifier_redeclaration(variable_name);
    this->_identifier_manager.add_variable(variable_name);
}

void compiler::declare_vararray(
    const std::string& vararray_name,
    const architecture::memory_size_type size
) {
    this->_assert_no_identifier_redeclaration(vararray_name);
    this->_identifier_manager.add_vararray(vararray_name, size);
}

void compiler::declare_procedure(const std::string& procedure_name) {
    this->_assert_no_identifier_redeclaration(procedure_name);
    this->_identifier_manager.add_procedure(procedure_name);
}

void compiler::set_identifier_used(const std::string& identifier_name) {
    this->_assert_identifier_defined(identifier_name);
    this->_identifier_manager.get(identifier_name)->set_used();
}

// TODO: read_varaiable

void compiler::print_value(const architecture::value_type value) {
    this->_asm_builder.write_value(value);
}

// TODO: write_variable

void compiler::assign_value_to_variable(
    const std::string& variable_name, architecture::vm_register& value_register
) {
    this->_assert_identifier_defined(variable_name, identifier_discriminator::variable);

    // const auto& variable = this->_identifier_manager.get(variable_name);
    // TODO: this->_asm_builder.store(variable->address(), value_register);
}

void compiler::assign_value_to_vararrray_element(
    const std::string& vararray_name,
    const architecture::memory_size_type vararray_idx,
    architecture::vm_register& value_register
) {
    this->_assert_identifier_defined(vararray_name, identifier_discriminator::vararray);

    // const auto& variable = this->_identifier_manager.get(vararray_name);
    // TODO: this->_asm_builder.store(variable->address() + varraray_idx, value_register);
}

void compiler::_assert_no_identifier_redeclaration(
    const std::string& identifier_name
) const {
    if (!this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
                << "\tIdentifier `" << identifier_name << "` already defined" << std::endl;
    std::exit(1);
}

void compiler::_assert_identifier_defined(const std::string& identifier_name) const {
    if (this->_identifier_manager.has(identifier_name))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
                << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

void compiler::_assert_identifier_defined(
    const std::string& identifier_name, const identifier_discriminator discriminator
) const {
    if (this->_identifier_manager.has(identifier_name, discriminator))
        return;

    std::cerr << "[ERROR] In line: " << this->_line_no << std::endl
                << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
    std::exit(1);
}

} // namespace jftt

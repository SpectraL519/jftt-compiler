#pragma once

#include "identifier_manager.hpp"
#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"


namespace jftt {

class compiler {
public:
    compiler() = default;
    ~compiler() = default;

    void declare_variable(const uint64_t line_no, const std::string& variable_name) noexcept {
        this->assert_no_identifier_redeclaration(line_no, variable_name);
        this->_identifier_manager.add_variable(variable_name);
    }

    void declare_vararray(
        const uint64_t line_no,
        const std::string& vararray_name,
        const architecture::memory_size_type size
    ) noexcept {
        this->assert_no_identifier_redeclaration(line_no, vararray_name);
        this->_identifier_manager.add_vararray(vararray_name, size);
    }

    void declare_procedure(const uint64_t line_no, const std::string& procedure_name) noexcept {
        this->assert_no_identifier_redeclaration(line_no, procedure_name);
        this->_identifier_manager.add_procedure(procedure_name);
    }

    void set_identifier_used(const uint64_t line_no, const std::string& identifier_name) noexcept {
        this->assert_identifier_defined(line_no, identifier_name);
        this->_identifier_manager.get(identifier_name)->set_used();
    }

    void assign_value(const uint64_t line_no, const std::string& variable_name) noexcept {
        this->assert_identifier_defined(line_no, variable_name, identifier_discriminator::variable);

        const auto& variable = this->_identifier_manager.get(variable_name);
        // TODO: this->_asm_builder.store(accumulator, variable->address());
    }

    void assign_value(
        const uint64_t line_no,
        const std::string& vararray_name,
        const architecture::memory_size_type vararray_idx
    ) noexcept {
        this->assert_identifier_defined(line_no, vararray_name, identifier_discriminator::vararray);

        const auto& variable = this->_identifier_manager.get(vararray_name);
        // TODO: this->_asm_builder.store(accumulator, variable->address() + varraray_idx);
    }

private:
    void assert_no_identifier_redeclaration(
        const uint64_t line_no, const std::string& identifier_name
    ) const noexcept {
        if (!this->_identifier_manager.has(identifier_name))
            return;

        std::cerr << "[ERROR] In line: " << line_no << std::endl
                  << "\tIdentifier `" << identifier_name << "` already defined" << std::endl;
        std::exit(1);
    }

    void assert_identifier_defined(
        const uint64_t line_no,
        const std::string& identifier_name
    ) const noexcept {
        if (this->_identifier_manager.has(identifier_name))
            return;

        std::cerr << "[ERROR] In line: " << line_no << std::endl
                  << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
        std::exit(1);
    }

    void assert_identifier_defined(
        const uint64_t line_no,
        const std::string& identifier_name,
        const identifier_discriminator discriminator
    ) const noexcept {
        if (this->_identifier_manager.has(identifier_name, discriminator))
            return;

        std::cerr << "[ERROR] In line: " << line_no << std::endl
                  << "\tUndefined identifier `" << identifier_name << '`' << std::endl;
        std::exit(1);
    }

    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
    identifier_manager _identifier_manager;
    assembly::code_builder _asm_builder;
};

} // namespace jftt

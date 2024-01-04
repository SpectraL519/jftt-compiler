#pragma once

#include "identifier_manager.hpp"
#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"


namespace jftt {

class compiler {
public:
    compiler() = default;
    ~compiler() = default;

    void read_input();

    void declare_variable(const uint64_t line_no, const std::string& variable_name);
    void declare_vararray(
        const uint64_t line_no,
        const std::string& vararray_name,
        const architecture::memory_size_type size
    );
    void declare_procedure(const uint64_t line_no, const std::string& procedure_name);

    void set_identifier_used(const uint64_t line_no, const std::string& identifier_name);

    void assign_value_to_variable(
        const uint64_t line_no,
        const std::string& variable_name,
        architecture::vm_register& value_register
    );
    void assign_value_to_vararrray_element(
        const uint64_t line_no,
        const std::string& vararray_name,
        const architecture::memory_size_type vararray_idx,
        architecture::vm_register& value_register
    );

private:
    void _assert_no_identifier_redeclaration(
        const uint64_t line_no, const std::string& identifier_name) const;
    void _assert_identifier_defined(
        const uint64_t line_no,
        const std::string& identifier_name) const;
    void _assert_identifier_defined(
        const uint64_t line_no,
        const std::string& identifier_name,
        const identifier_discriminator discriminator) const;

    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
    identifier_manager _identifier_manager;
    assembly::code_builder _asm_builder;
};

} // namespace jftt

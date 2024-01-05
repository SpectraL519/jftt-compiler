#pragma once

#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"
#include "identifier_manager.hpp"


namespace jftt {

class compiler {
public:
    compiler() = default;
    ~compiler() = default;

    void finish_code_generating();
    const std::vector<std::string>& asm_code() const;

    void set_line_no(const std::size_t line_no);

    void declare_variable(const std::string& variable_name);
    void declare_vararray(
        const std::string& vararray_name,
        const architecture::memory_size_type size
    );
    void declare_procedure(const std::string& procedure_name);

    [[nodiscard]] identifier::abstract_identifier* get_identifier(const std::string& name);
    // void set_identifier_used(const std::string& identifier_name);

    [[nodiscard]] architecture::memory_address_type get_address(const std::string& variable_name);

    void scan(identifier::abstract_identifier* identifier);
    void print(identifier::abstract_identifier* identifier);

    void assign_value_to_variable(
        const std::string& variable_name, architecture::vm_register& value_register);
    void assign_value_to_vararrray_element(
        const std::string& vararray_name,
        const architecture::memory_size_type vararray_idx,
        architecture::vm_register& value_register);

private:
    void _assert_no_identifier_redeclaration(const std::string& identifier_name) const;
    void _assert_identifier_defined(const std::string& identifier_name) const;
    void _assert_identifier_defined(
        const std::string& identifier_name,
        const identifier_discriminator discriminator) const;

    std::size_t _line_no{1u};
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
    identifier_manager _identifier_manager;
    assembly::code_builder _asm_builder;
};

} // namespace jftt

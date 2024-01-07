#pragma once

#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"
#include "condition/branch_manager.hpp"
#include "identifier_manager.hpp"
#include "loop_manager.hpp"



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

    [[nodiscard]] const std::shared_ptr<identifier::abstract_identifier>& get_identifier(
        const std::string& name);

    void initialize_lvalue_identifier(const std::string& name);

    void scan(identifier::abstract_identifier* identifier);
    void print(identifier::abstract_identifier* identifier);

    void acquire_accumulator();
    void release_accumulator();

    void return_value(identifier::abstract_identifier* identifier);
    void assign_value_to(identifier::abstract_identifier* identifier);

    void add_condition(
        const condition_discriminator discriminator,
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b);
    void end_latest_condition_without_else();
    void end_latest_condition_with_else();

    void add_loop(const loop_discriminator discriminator);
    void set_latest_while_loop_end_label();
    void end_loop(const loop_discriminator discriminator);

    void add(identifier::abstract_identifier* a, identifier::abstract_identifier* b);
    void subtract(identifier::abstract_identifier* a, identifier::abstract_identifier* b);
    void multiply(identifier::abstract_identifier* a, identifier::abstract_identifier* b);
    void divide(identifier::abstract_identifier* a, identifier::abstract_identifier* b);
    void modulo(identifier::abstract_identifier* a, identifier::abstract_identifier* b);

    void assert_no_identifier_redeclaration(const std::string& identifier_name) const;
    void assert_identifier_defined(const std::string& identifier_name) const;
    void assert_identifier_defined(
        const std::string& identifier_name,
        const identifier_discriminator discriminator) const;
    void assert_lvalue_initialized(const std::string& lvalue_name) const;

private:
    std::size_t _line_no{1u};
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
    assembly::code_builder _asm_builder;
    condition::branch_manager _condition_manager;
    identifier_manager _identifier_manager;
    loop_manager _loop_manager;
};

} // namespace jftt

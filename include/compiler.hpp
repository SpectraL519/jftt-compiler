#pragma once

#include "architecture/vm_memory_manager.hpp"
#include "assembly/code_builder.hpp"
#include "condition/branch_manager.hpp"
#include "identifier_manager.hpp"
#include "loop_manager.hpp"

#include <optional>
#include <vector>



namespace jftt {

class compiler {
public:
    compiler() = default;
    ~compiler() = default;

    void with_procedurers();
    void begin_program();
    void finish_code_generating();
    const std::vector<std::string>& asm_code() const;

    void set_line_no(const std::size_t line_no);

    void declare_variable(
        const std::string& variable_name,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void declare_vararray(
        const std::string& vararray_name,
        const architecture::memory_size_type size,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void declare_procedure(const std::string& procedure_name);

    void declare_procedure_parameter(
        const std::string& procedure_name,
        const identifier_discriminator param_discriminator,
        const std::string& local_name);
    void begin_procedure_implementation(const std::string& procedure_name);
    void pass_procedure_parameter(
        const std::string& procedure_name,
        identifier::abstract_identifier* lvalue,
        const std::optional<std::string>& current_procedure);
    void end_procedure_call_args_declaration(const std::string& procedure_name);

    void return_from_procedure(const std::string& procedure_name);
    void call_procedure(const std::string& procedure_name);

    [[nodiscard]] std::shared_ptr<identifier::abstract_identifier> get_identifier(
        const std::string& name,
        const std::optional<std::string>& procedure_name = std::nullopt);

    void initialize_lvalue_identifier(
        const std::string& name,
        const std::optional<std::string>& procedure_name = std::nullopt);

    void scan(
        identifier::abstract_identifier* identifier,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void print(
        identifier::abstract_identifier* identifier,
        const std::optional<std::string>& procedure_name = std::nullopt);

    void acquire_accumulator();
    void release_accumulator();

    void return_value(
        identifier::abstract_identifier* identifier,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assign_value_to(
        identifier::abstract_identifier* identifier,
        const std::optional<std::string>& procedure_name = std::nullopt);

    void add_condition(
        const condition_discriminator discriminator,
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void end_latest_condition_without_else();
    void end_latest_condition_with_else();

    void add_loop(const loop_discriminator discriminator);
    void set_latest_loop_end_label();
    void end_loop(const loop_discriminator discriminator);

    void add(
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void subtract(
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void multiply(
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void divide(
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void modulo(
        identifier::abstract_identifier* a,
        identifier::abstract_identifier* b,
        const std::optional<std::string>& procedure_name = std::nullopt);

    // asserts should be const
    void assert_no_identifier_redeclaration(
        const std::string& identifier_name,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assert_identifier_defined(
        const std::string& identifier_name,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assert_identifier_defined(
        const std::string& identifier_name,
        const identifier_discriminator discriminator,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assert_identifier_defined(
        const std::string& identifier_name,
        const std::vector<identifier_discriminator>& discriminator_list,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assert_lvalue_initialized(
        const std::string& identifier_name,
        const identifier_discriminator discriminator,
        const std::optional<std::string>& procedure_name = std::nullopt);
    void assert_index_in_range(
        const std::string& vararray_name,
        const architecture::value_type index,
        const std::optional<std::string>& procedure_name = std::nullopt);

    void throw_error(const std::string& msg) const;

private:
    void _warn_uninitialized_identifier_condition(
        const std::string& identifier_name,
        const std::optional<std::string>& procedure_name = std::nullopt);

    std::size_t _line_no{1u};
    std::optional<std::string> _program_begin_label;

    assembly::code_builder _asm_builder;

    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
    condition::branch_manager _condition_manager;
    identifier_manager _identifier_manager;
    loop_manager _loop_manager;
};

} // namespace jftt

#pragma once

#include "instructions.hpp"
#include "jump_manager.hpp"
#include "condition/branch.hpp"
#include "../identifier.hpp"
#include "../loop.hpp"
#include "../architecture/vm_memory_manager.hpp"

#include <vector>



namespace jftt::assembly {

class jump_manager;

class code_builder {
public:
    code_builder();

    ~code_builder() = default;

    void add_instruction(const std::string& instruction);
    void stop_building();
    [[nodiscard]] const std::vector<std::string>& code() const;
    [[nodiscard]] const std::size_t current_line() const;

    void read_lvalue(const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue);
    void write_identifier(const std::shared_ptr<identifier::abstract_identifier>& identifier);

    void initialize_identifier_value_in_register(
        const std::shared_ptr<identifier::abstract_identifier>& identifier,
        architecture::vm_register& reg);
    void initialize_value_in_register(
        const architecture::value_type value, architecture::vm_register& reg);
    void initialize_addres_in_register(
        const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue,
        architecture::vm_register& reg);

    void load_value_from_address(architecture::vm_register& value_register);
    void store_value_from_register(
        architecture::vm_register& value_register, architecture::vm_register& address_register);

    architecture::vm_register& move_acc_content_to_tmp_register();
    void move_tmp_register_content_to_acc(architecture::vm_register& tmp_register);

    [[nodiscard]] std::string new_condition_jump_label(const std::string& label_name);
    void new_condition_branch_jump_point(const condition::branch& branch);
    void set_condition_branch_jump_point(const condition::branch& branch);
    void set_while_loop_begin_label(const loop::abstract_loop& loop);
    void set_while_loop_end_label(const loop::abstract_loop& loop);

    [[nodiscard]] condition::branch equal_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);
    [[nodiscard]] condition::branch not_equal_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);
    [[nodiscard]] condition::branch less_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);
    [[nodiscard]] condition::branch less_equal_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);
    [[nodiscard]] condition::branch greater_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);
    [[nodiscard]] condition::branch greater_equal_condition(
        architecture::vm_register& a_register, architecture::vm_register& b_register);

    void subtract(
        const std::shared_ptr<identifier::abstract_identifier>& a,
        const std::shared_ptr<identifier::abstract_identifier>& b);
    void multiply(
        const std::shared_ptr<identifier::abstract_identifier>& a,
        const std::shared_ptr<identifier::abstract_identifier>& b);
    void divide(
        const std::shared_ptr<identifier::abstract_identifier>& a,
        const std::shared_ptr<identifier::abstract_identifier>& b);
    void modulo(
        const std::shared_ptr<identifier::abstract_identifier>& a,
        const std::shared_ptr<identifier::abstract_identifier>& b);

    friend class assembly::jump_manager;

private:
    void _write_lvalue(const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue);
    void _write_rvalue(const architecture::value_type value);

    std::vector<std::string> _code;
    jump_manager _jump_manager;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt::assembly

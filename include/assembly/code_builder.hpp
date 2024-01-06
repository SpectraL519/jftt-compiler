#pragma once

#include "instructions.hpp"
#include "../identifier.hpp"
#include "../architecture/vm_memory_manager.hpp"

#include <vector>



namespace jftt::assembly {

class code_builder {
public:
    code_builder() = default;
    ~code_builder() = default;

    void add_instruction(const std::string& instruction);
    void stop_building();
    const std::vector<std::string>& code() const;

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

    architecture::vm_register& _move_acc_content_to_tmp_register();
    void _move_tmp_register_content_to_acc(architecture::vm_register& tmp_register);

private:
    void _write_lvalue(const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue);
    void _write_rvalue(const architecture::value_type value);

    std::vector<std::string> _code;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt::assembly

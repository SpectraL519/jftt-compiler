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

    void stop_building();
    const std::vector<std::string>& code() const;

    void read_variable(const std::shared_ptr<identifier::variable>& variable);
    void read_vararray_element(const std::shared_ptr<identifier::vararray>& vararray);
    void write_value(const architecture::value_type value);
    void write_variable(const architecture::memory_address_type address);

    void initialize_value_in_register(
        const architecture::value_type value, architecture::vm_register& reg);
    void store_value_from_register(
        architecture::vm_register& value_register, const architecture::memory_address_type address);

private:
    architecture::vm_register& _move_acc_to_tmp_register();
    void _move_tmp_register_to_acc(architecture::vm_register& tmp_register);

    void _add_instruction(const std::string& instruction);

    std::vector<std::string> _code;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt::assembly

#pragma once

#include "instructions.hpp"
#include "../architecture/vm_memory_manager.hpp"

#include <vector>



namespace jftt::assembly {

class code_builder {
public:
    code_builder() = default;
    ~code_builder() = default;

    void initialize_value_in_register(
        const architecture::value_type value, architecture::vm_register& reg);

    void store_value_from_register(
        architecture::vm_register& value_register, const architecture::memory_address_type address);

private:
    void _add_instruction(const std::string& instruction);

    std::vector<std::string> _code;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt::assembly

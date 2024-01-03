#pragma once

#include "instructions.hpp"
#include "../architecture/vm_memory_manager.hpp"

#include <bitset>
#include <vector>


namespace jftt::assembly {

class code_builder {
public:
    code_builder() = default;
    ~code_builder() = default;

    void initialize_value_in_register(
        const architecture::value_type value, architecture::vm_register& reg
    ) {
        this->add_instruction(instructions::rst(reg));

        // get the bit representation of the value
        std::bitset<architecture::VM_VALUE_BITSIZE> bits{value};

        int idx{architecture::VM_VALUE_BITSIZE};
        while (idx > 0 && !bits[--idx]) continue; // skip oldest bits until 1 is found

        // initialize the value bit by bit
        while (idx > 0) {
            if (bits[idx])
                this->add_instruction(instructions::inc(reg));
            this->add_instruction(instructions::shl(reg));
        }
        if (bits[idx])
            this->add_instruction(instructions::inc(reg));
    }

    void store(
        architecture::vm_register& value_register,
        const architecture::memory_address_type address
    ) {
        auto& address_register{this->_memory_manager.get_free_register()};
        address_register.acquire();

        if (this->_memory_manager.get_accumulator().is_free()) {
            // store value from acc to the given addres
            this->add_instruction(instructions::get(value_register));
            this->add_instruction(instructions::store(address_register));
        }
        else {
            // store acc value in a temporary register
            auto& tmp_register{this->_memory_manager.get_free_register()};
            tmp_register.acquire();
            this->add_instruction(instructions::put(tmp_register));

            // store value from acc to the given addres
            this->add_instruction(instructions::get(value_register));
            this->add_instruction(instructions::store(address_register));

            // move value from temporary register back to acc
            this->add_instruction(instructions::get(tmp_register));
            tmp_register.release();
        }

        address_register.release();
    }

private:
    void add_instruction(const std::string& instruction) {
        this->_code.emplace_back(instruction);
    }

    std::vector<std::string> _code;
    architecture::vm_memory_manager& _memory_manager{architecture::vm_memory_manager::instance()};
};

} // namespace jftt::assembly

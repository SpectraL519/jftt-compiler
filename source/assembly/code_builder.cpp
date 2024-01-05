#include "assembly/code_builder.hpp"

#include "architecture/vm_memory_manager.hpp"
#include "assembly/instructions.hpp"

#include <bitset>
#include <iostream>



namespace jftt::assembly {

void code_builder::stop_building() {
    this->_add_instruction(instructions::halt());
}

const std::vector<std::string>& code_builder::code() const {
    return this->_code;
}

void code_builder::read_variable(const std::shared_ptr<identifier::variable>& variable) {
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();
    this->initialize_value_in_register(variable->address(), address_register);

    this->_add_instruction(instructions::read());
    this->_add_instruction(instructions::store(address_register));

    address_register.release();
}

void code_builder::read_vararray_element(const std::shared_ptr<identifier::vararray>& vararray) {
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();
    this->initialize_value_in_register(vararray->address(), address_register);

    // TODO: read at index

    this->_add_instruction(instructions::read());
    this->_add_instruction(instructions::store(address_register));

    address_register.release();
}

void code_builder::write_value(const architecture::value_type value) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    if (accumulator.is_free()) {
        this->initialize_value_in_register(value, accumulator);
        this->_add_instruction(instructions::write());
    }
    else {
        auto& tmp_register{this->_move_acc_to_tmp_register()};
        this->initialize_value_in_register(value, accumulator);
        this->_add_instruction(instructions::write());
        this->_move_tmp_register_to_acc(tmp_register);
    }
}

void code_builder::write_variable(const architecture::memory_address_type address) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    if (accumulator.is_free()) {
        this->initialize_value_in_register(address, accumulator);
        this->_add_instruction(instructions::load(accumulator));
        this->_add_instruction(instructions::write());
    }
    else {
        auto& tmp_register{this->_move_acc_to_tmp_register()};
        this->initialize_value_in_register(address, accumulator);
        this->_add_instruction(instructions::load(accumulator));
        this->_add_instruction(instructions::write());
        this->_move_tmp_register_to_acc(tmp_register);
    }
}

void code_builder::initialize_value_in_register(
    const architecture::value_type value, architecture::vm_register& reg
) {
    this->_add_instruction(instructions::rst(reg));

    // get the bit representation of the value
    std::bitset<architecture::VM_VALUE_BITSIZE> bits{value};

    int idx{architecture::VM_VALUE_BITSIZE};
    while (idx > 0 && !bits[--idx]) continue; // skip oldest bits until 1 is found

    // initialize the value bit by bit
    while (idx > 0) {
        if (bits[idx--])
            this->_add_instruction(instructions::inc(reg));
        this->_add_instruction(instructions::shl(reg));
    }
    if (bits[idx])
        this->_add_instruction(instructions::inc(reg));
}

void code_builder::store_value_from_register(
    architecture::vm_register& value_register, const architecture::memory_address_type address
) {
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();
    this->initialize_value_in_register(address, address_register);

    if (this->_memory_manager.get_accumulator().is_free()) {
        this->_add_instruction(instructions::get(value_register));
        this->_add_instruction(instructions::store(address_register));
    }
    else {
        auto& tmp_register{this->_move_acc_to_tmp_register()};
        this->_add_instruction(instructions::get(value_register));
        this->_add_instruction(instructions::store(address_register));
        this->_move_tmp_register_to_acc(tmp_register);
    }

    address_register.release();
}

architecture::vm_register& code_builder::_move_acc_to_tmp_register() {
    auto& tmp_register{this->_memory_manager.get_free_register()};
    tmp_register.acquire();
    this->_add_instruction(instructions::put(tmp_register));
    return tmp_register;
}

void code_builder::_move_tmp_register_to_acc(architecture::vm_register& tmp_register) {
    this->_add_instruction(instructions::get(tmp_register));
    tmp_register.release();
}

void code_builder::_add_instruction(const std::string& instruction) {
    this->_code.emplace_back(instruction);
}

} // namespace jftt::assembly

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

void code_builder::read_lvalue(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue
) {
    std::cout << "1";

    // make sure accumulator is free
    auto& accumulator{this->_memory_manager.get_accumulator()};
    architecture::vm_register* tmp_register{nullptr};
    if (!accumulator.is_free())
        tmp_register = &this->_move_acc_content_to_tmp_register();

    std::cout << "2";

    // initialize the address of the beggining of the array in a free register
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();

    std::cout << "3";

    // read the array element
    this->initialize_addres_in_register(lvalue, address_register);
    this->_add_instruction(instructions::read());
    this->_add_instruction(instructions::store(address_register));
    address_register.release();

    std::cout << "4";

    if (tmp_register)
        this->_move_tmp_register_content_to_acc(*tmp_register);

    std::cout << "5\n";
}

void code_builder::write_lvalue(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue
) {
    // make sure accumulator is free
    auto& accumulator{this->_memory_manager.get_accumulator()};
    architecture::vm_register* tmp_register{nullptr};
    if (!accumulator.is_free())
        tmp_register = &this->_move_acc_content_to_tmp_register();

    // initialize the address of the beggining of the array in a free register
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();

    // read the array element
    this->initialize_addres_in_register(lvalue, address_register);
    this->_add_instruction(instructions::load(address_register));
    this->_add_instruction(instructions::write());
    address_register.release();

    if (tmp_register)
        this->_move_tmp_register_content_to_acc(*tmp_register);
}

void code_builder::write_rvalue(const architecture::value_type value) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    if (accumulator.is_free()) {
        this->initialize_value_in_register(value, accumulator);
        this->_add_instruction(instructions::write());
    }
    else {
        auto& tmp_register{this->_move_acc_content_to_tmp_register()};
        this->initialize_value_in_register(value, accumulator);
        this->_add_instruction(instructions::write());
        this->_move_tmp_register_content_to_acc(tmp_register);
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

void code_builder::initialize_addres_in_register(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue,
    architecture::vm_register& reg
) {
    std::cout << "1";
    if (lvalue->discriminator() == identifier_discriminator::variable) {
        this->initialize_value_in_register(lvalue->address(), reg);
        return;
    }

    std::cout << "2";

    // TODO: replace dynamic_cast here with a casting function
    const auto vararray{
        identifier::shared_ptr_cast<identifier_discriminator::vararray>(lvalue)};
    this->initialize_value_in_register(vararray->address(), reg);

    std::cout << "3";

    // initialize the index value in the accumulator
    auto& accumulator{this->_memory_manager.get_accumulator()};
    const auto& indexer{vararray->indexer()};
    if (indexer->discriminator() == identifier_discriminator::rvalue) {
        // acc = index_value
        this->initialize_value_in_register(
            identifier::shared_ptr_cast<identifier_discriminator::rvalue>(indexer)->value(),
            accumulator);
    }
    else {
        // acc = *index_variable
        this->initialize_value_in_register(
            identifier::shared_ptr_cast<identifier_discriminator::variable>(indexer)->address(),
            accumulator);
        this->_add_instruction(instructions::load(accumulator));
    }

    std::cout << "4";

    // add the array begin addres to get the addres of element at index
    this->_add_instruction(instructions::add(reg));

    // store the final address in the desired register
    this->_add_instruction(instructions::put(reg));

    std::cout << "5";
}

void code_builder::load_value_from_address(
    const architecture::memory_address_type address, architecture::vm_register& address_register
) {
    auto& value_register{this->_memory_manager.get_free_register()};
    value_register.acquire();

    if (this->_memory_manager.get_accumulator().is_free()) {
        this->_add_instruction(instructions::load(address_register));
        this->_add_instruction(instructions::put(value_register));
    }
    else {
        auto& tmp_register{this->_move_acc_content_to_tmp_register()};
        this->_add_instruction(instructions::load(address_register));
        this->_add_instruction(instructions::put(value_register));
        this->_move_tmp_register_content_to_acc(tmp_register);
    }

    // ? value_register.release();
}


void code_builder::store_value_from_register(
    architecture::vm_register& value_register, const architecture::memory_address_type address
) {
    auto& address_register{this->_memory_manager.get_free_register()};
    address_register.acquire();
    this->initialize_value_in_register(address, address_register);

    if (architecture::is_accumulator(value_register)) {
        this->_add_instruction(instructions::store(address_register));
        return;
    }

    if (this->_memory_manager.get_accumulator().is_free()) {
        this->_add_instruction(instructions::get(value_register));
        this->_add_instruction(instructions::store(address_register));
    }
    else {
        auto& tmp_register{this->_move_acc_content_to_tmp_register()};
        this->_add_instruction(instructions::get(value_register));
        this->_add_instruction(instructions::store(address_register));
        this->_move_tmp_register_content_to_acc(tmp_register);
    }

    address_register.release();
}

architecture::vm_register& code_builder::_move_acc_content_to_tmp_register() {
    auto& tmp_register{this->_memory_manager.get_free_register()};
    tmp_register.acquire();
    this->_add_instruction(instructions::put(tmp_register));
    return tmp_register;
}

void code_builder::_move_tmp_register_content_to_acc(architecture::vm_register& tmp_register) {
    this->_add_instruction(instructions::get(tmp_register));
    tmp_register.release();
}

void code_builder::_add_instruction(const std::string& instruction) {
    this->_code.emplace_back(instruction);
}

} // namespace jftt::assembly

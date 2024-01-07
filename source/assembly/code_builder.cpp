#include "assembly/code_builder.hpp"
#include "assembly/jump_manager.hpp"

#include "architecture/vm_memory_manager.hpp"
#include "assembly/instructions.hpp"

#include <algorithm>
#include <bitset>
#include <iostream>



namespace jftt::assembly {

code_builder::code_builder()
: _jump_manager(jump_manager{*this}) {}

void code_builder::add_instruction(const std::string& instruction) {
    this->_code.emplace_back(instruction);
}

void code_builder::stop_building() {
    this->_jump_manager.fill_labels();
    this->add_instruction(instructions::halt());
}

const std::vector<std::string>& code_builder::code() const {
    return this->_code;
}

const std::size_t code_builder::current_line() const {
    return this->_code.size();
}

void code_builder::read_lvalue(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue
) {
    // make sure accumulator is free
    auto& accumulator{this->_memory_manager.get_accumulator()};
    architecture::vm_register* tmp_register{nullptr};
    if (!accumulator.is_free())
        tmp_register = &this->move_acc_content_to_tmp_register();

    // initialize the address of the beggining of the array in a free register
    auto& address_register{this->_memory_manager.acquire_free_register()};

    // read the array element
    this->initialize_addres_in_register(lvalue, address_register);
    this->add_instruction(instructions::read());
    this->add_instruction(instructions::store(address_register));
    address_register.release();

    if (tmp_register != nullptr)
        this->move_tmp_register_content_to_acc(*tmp_register);
}

void code_builder::write_identifier(
    const std::shared_ptr<identifier::abstract_identifier>& identifier
) {
    if (identifier->discriminator() == identifier_discriminator::rvalue) {
        this->_write_rvalue(
            identifier::shared_ptr_cast<identifier_discriminator::rvalue>(identifier)->value());
    }
    else {
        this->_write_lvalue(
            identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier));
    }
}

void code_builder::initialize_identifier_value_in_register(
    const std::shared_ptr<identifier::abstract_identifier>& identifier,
    architecture::vm_register& reg
) {
    if (identifier->discriminator() == identifier_discriminator::rvalue) {
        this->initialize_value_in_register(
            identifier::shared_ptr_cast<identifier_discriminator::rvalue>(identifier)->value(),
            reg);
    }
    else {
        if (architecture::is_accumulator(reg)) {
            this->initialize_addres_in_register(
                identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier), reg);
            this->add_instruction(instructions::load(reg));
        }
        else {
            architecture::vm_register* tmp_register{nullptr};
            if (!this->_memory_manager.get_accumulator().is_free())
                tmp_register = &this->move_acc_content_to_tmp_register();

            this->initialize_addres_in_register(
                identifier::shared_ptr_cast<identifier_discriminator::lvalue>(identifier), reg);
            this->add_instruction(instructions::load(reg));
            this->add_instruction(instructions::put(reg));

            if (tmp_register != nullptr)
                this->move_tmp_register_content_to_acc(*tmp_register);
        }
    }
}

void code_builder::initialize_value_in_register(
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
        idx--;
    }
    if (bits[idx])
        this->add_instruction(instructions::inc(reg));
}

void code_builder::initialize_addres_in_register(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue,
    architecture::vm_register& reg
) {
    if (lvalue->discriminator() == identifier_discriminator::variable) {
        this->initialize_value_in_register(lvalue->address(), reg);
        return;
    }

    const auto vararray{
        identifier::shared_ptr_cast<identifier_discriminator::vararray>(lvalue)};
    auto& address_register{this->_memory_manager.acquire_free_register()};
    this->initialize_value_in_register(vararray->address(), address_register);

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
        this->add_instruction(instructions::load(accumulator));
    }

    // add the array[0] address to the index to get array[index] address
    this->add_instruction(instructions::add(address_register));
    address_register.release();
    if (!architecture::is_accumulator(reg))
        this->add_instruction(instructions::put(reg));
}

// TODO: pass value register
void code_builder::load_value_from_address(architecture::vm_register& address_register) {
    auto& value_register{this->_memory_manager.acquire_free_register()};

    if (this->_memory_manager.get_accumulator().is_free()) {
        this->add_instruction(instructions::load(address_register));
        this->add_instruction(instructions::put(value_register));
    }
    else {
        auto& tmp_register{this->move_acc_content_to_tmp_register()};
        this->add_instruction(instructions::load(address_register));
        this->add_instruction(instructions::put(value_register));
        this->move_tmp_register_content_to_acc(tmp_register);
    }

    value_register.release();
}

void code_builder::store_value_from_register(
    architecture::vm_register& value_register, architecture::vm_register& address_register
) {
    if (architecture::is_accumulator(value_register)) {
        this->add_instruction(instructions::store(address_register));
        return;
    }

    if (this->_memory_manager.get_accumulator().is_free()) {
        this->add_instruction(instructions::get(value_register));
        this->add_instruction(instructions::store(address_register));
    }
    else {
        auto& tmp_register{this->move_acc_content_to_tmp_register()};
        this->add_instruction(instructions::get(value_register));
        this->add_instruction(instructions::store(address_register));
        this->move_tmp_register_content_to_acc(tmp_register);
    }
}

architecture::vm_register& code_builder::move_acc_content_to_tmp_register() {
    auto& tmp_register{this->_memory_manager.acquire_free_register()};
    this->add_instruction(instructions::put(tmp_register));
    return tmp_register;
}

void code_builder::move_tmp_register_content_to_acc(architecture::vm_register& tmp_register) {
    this->add_instruction(instructions::get(tmp_register));
    tmp_register.release();
}

condition::branch code_builder::equal_condition(
    architecture::vm_register& a_register, architecture::vm_register& b_register
) {
    const std::string true_label{this->_jump_manager.new_label("eq_true")};
    const std::string false_label{this->_jump_manager.new_label("eq_false")};

    // double check required: a - b == 0 and b - a == 0
    const std::string double_check_label{this->_jump_manager.new_label("eq_double_check")};

    auto& accumulator{this->_memory_manager.get_accumulator()};

    // check a - b == 0
    this->add_instruction(instructions::get(a_register));
    this->add_instruction(instructions::sub(b_register));
    // if true: perform double check
    this->_jump_manager.jump_to_label(instructions::jzero_label, double_check_label);
    // if false: return false
    this->_jump_manager.jump_to_label(instructions::jump_label, false_label);

    // check b - a == 0
    this->_jump_manager.insert_label(double_check_label);
    this->add_instruction(instructions::get(b_register));
    this->add_instruction(instructions::sub(a_register));
    // if true: return true
    this->_jump_manager.jump_to_label(instructions::jzero_label, true_label);
    // if false: return false
    this->_jump_manager.jump_to_label(instructions::jump_label, false_label);

    // 'if true' code will be inserted just below the true_label
    this->_jump_manager.insert_label(true_label);

    // 'if false' code will be inserted just below the true_label
    return condition::branch{false_label};
}

// TODO: other conditions

void code_builder::end_condition(const condition::branch& branch, const bool with_else) {
    if (with_else)
        this->_jump_manager.jump_to_label(instructions::jump_label, branch.false_eval_label);
    else
        this->_jump_manager.insert_label(branch.false_eval_label);
}

void code_builder::multiply(
    architecture::vm_register& a_register, architecture::vm_register& b_register
) {
    // TODO: use conditions and loops
    auto& is_odd_register{this->_memory_manager.acquire_free_register()};
    auto& result_register{this->_memory_manager.acquire_free_register()};

    const std::string end_label{this->_jump_manager.new_label("mul_end")};
    const std::string loop_begin_label{this->_jump_manager.new_label("mul_loop")};
    const std::string inside_loop_label{this->_jump_manager.new_label("mul_insied_loop")};
    const std::string is_odd_label{this->_jump_manager.new_label("mul_is_odd")};

    this->add_instruction(instructions::rst(result_register));

    // check a == 0
    this->add_instruction(instructions::get(a_register));
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);

    // begin multiplication loop
    this->_jump_manager.insert_label(loop_begin_label);

    // check b == 0 (initial and per iteration check)
    this->add_instruction(instructions::get(b_register));
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);

    this->add_instruction(instructions::put(is_odd_register)); // x = b
    this->add_instruction(instructions::shr(is_odd_register)); // x >> 1
    this->add_instruction(instructions::shl(is_odd_register)); // x << 1 (get rid of last bit)
    this->add_instruction(instructions::sub(is_odd_register)); // b = b - x
    this->_jump_manager.jump_to_label(instructions::jpos_label, is_odd_label);
    this->_jump_manager.insert_label(inside_loop_label);
    this->add_instruction(instructions::shl(a_register)); // a = a * 2
    this->add_instruction(instructions::shr(b_register)); // b = b / 2
    this->_jump_manager.jump_to_label(instructions::jump_label, loop_begin_label);

    this->_jump_manager.insert_label(is_odd_label);
    this->add_instruction(instructions::get(result_register));
    this->add_instruction(instructions::add(a_register));
    this->add_instruction(instructions::put(result_register));
    this->_jump_manager.jump_to_label(instructions::jump_label, inside_loop_label);

    // end loop
    this->_jump_manager.insert_label(end_label);
    // return result (move to acc)
    this->add_instruction(instructions::get(result_register));

    is_odd_register.release();
    result_register.release();
}

void code_builder::divide(
    architecture::vm_register& a_register, architecture::vm_register& b_register
) {
    // TODO
}

void code_builder::_write_lvalue(
    const std::shared_ptr<identifier::abstract_lvalue_identifier>& lvalue
) {
    // make sure accumulator is free
    auto& accumulator{this->_memory_manager.get_accumulator()};
    architecture::vm_register* tmp_register{nullptr};
    if (!accumulator.is_free())
        tmp_register = &this->move_acc_content_to_tmp_register();

    // initialize the address of the beggining of the array in a free register
    auto& address_register{this->_memory_manager.acquire_free_register()};

    // read the array element
    this->initialize_addres_in_register(lvalue, address_register);
    this->add_instruction(instructions::load(address_register));
    this->add_instruction(instructions::write());
    address_register.release();

    if (tmp_register != nullptr)
        this->move_tmp_register_content_to_acc(*tmp_register);
}

void code_builder::_write_rvalue(const architecture::value_type value) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    if (accumulator.is_free()) {
        this->initialize_value_in_register(value, accumulator);
        this->add_instruction(instructions::write());
    }
    else {
        auto& tmp_register{this->move_acc_content_to_tmp_register()};
        this->initialize_value_in_register(value, accumulator);
        this->add_instruction(instructions::write());
        this->move_tmp_register_content_to_acc(tmp_register);
    }
}

} // namespace jftt::assembly

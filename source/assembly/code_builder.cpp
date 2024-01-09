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


architecture::vm_register& code_builder::move_acc_content_to_tmp_register() {
    auto& tmp_register{this->_memory_manager.acquire_free_register()};
    this->add_instruction(instructions::put(tmp_register));
    return tmp_register;
}

void code_builder::move_tmp_register_content_to_acc(architecture::vm_register& tmp_register) {
    this->add_instruction(instructions::get(tmp_register));
    tmp_register.release();
}

std::string code_builder::new_jump_label(const std::string& label_name) {
    return this->_jump_manager.new_label(label_name);
}

// TODO: come up with a better name
void code_builder::insert_jump_point_label(const std::string& label) {
    this->_jump_manager.insert_label(label);
}

// TODO: come up with a better name
void code_builder::set_jump_point(const std::string& label) {
    this->_jump_manager.jump_to_label(instructions::jump_label, label);
}

void code_builder::start_loop(const std::shared_ptr<loop::abstract_loop>& loop) {
    this->_jump_manager.insert_label(loop->begin_label());
}

void code_builder::end_while_loop(const std::shared_ptr<loop::abstract_loop>& loop) {
    this->_jump_manager.jump_to_label(instructions::jump_label, loop->begin_label());
    this->_jump_manager.insert_label(loop->end_label());
}

void code_builder::end_repeat_until_loop(
    const std::shared_ptr<loop::abstract_loop>& loop
) {
    const std::string end_label{this->_jump_manager.new_label("repeat_until_loop_end")};
    this->_jump_manager.jump_to_label(instructions::jump_label, end_label);

    this->_jump_manager.insert_label(loop->end_label());
    this->_jump_manager.jump_to_label(instructions::jump_label, loop->begin_label());

    this->_jump_manager.insert_label(end_label);
}

condition::branch code_builder::equal_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    const std::string true_label{this->_jump_manager.new_label("eq_true")};
    const std::string false_label{this->_jump_manager.new_label("eq_false")};
    // double check required: a - b == 0 and b - a == 0
    const std::string double_check_label{this->_jump_manager.new_label("eq_double_check")};

    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    this->initialize_identifier_value_in_register(a, a_register);
    this->initialize_identifier_value_in_register(b, b_register);

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

    a_register.release();
    b_register.release();

    // 'if false' code will be inserted just below the true_label
    return condition::branch{false_label};
}

condition::branch code_builder::not_equal_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    const std::string true_label{this->_jump_manager.new_label("neq_true")};
    const std::string false_label{this->_jump_manager.new_label("neq_false")};
    // double check required: a - b == 0 and b - a == 0
    const std::string double_check_label{this->_jump_manager.new_label("neq_double_check")};

    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    this->initialize_identifier_value_in_register(a, a_register);
    this->initialize_identifier_value_in_register(b, b_register);

    // check a - b == 0
    this->add_instruction(instructions::get(a_register));
    this->add_instruction(instructions::sub(b_register));
    // if true: perform double check
    this->_jump_manager.jump_to_label(instructions::jzero_label, double_check_label);
    // if false: return true (a != b)
    this->_jump_manager.jump_to_label(instructions::jump_label, true_label);

    // check b - a == 0
    this->_jump_manager.insert_label(double_check_label);
    this->add_instruction(instructions::get(b_register));
    this->add_instruction(instructions::sub(a_register));
    // if true: return false (a == b)
    this->_jump_manager.jump_to_label(instructions::jzero_label, false_label);
    // if false: return false
    this->_jump_manager.jump_to_label(instructions::jump_label, true_label);

    // 'if true' code will be inserted just below the true_label
    this->_jump_manager.insert_label(true_label);

    a_register.release();
    b_register.release();

    // 'if false' code will be inserted just below the true_label
    return condition::branch{false_label};
}

condition::branch code_builder::less_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    // a < b <==> b > a
    return this->greater_condition(b, a);
}

condition::branch code_builder::less_equal_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    const std::string true_label{this->_jump_manager.new_label("leq_true")};
    const std::string false_label{this->_jump_manager.new_label("leq_false")};

    auto& accumulator{this->_memory_manager.get_accumulator()};

    // if a == 0: return true
    this->initialize_identifier_value_in_register(a, accumulator);
    this->_jump_manager.jump_to_label(instructions::jzero_label, true_label);

    // if a > 0: check condition
    accumulator.acquire();
    auto& b_register{this->_memory_manager.acquire_free_register()};
    this->initialize_identifier_value_in_register(b, b_register);
    accumulator.release();

    // check a - b == 0
    this->add_instruction(instructions::sub(b_register));
    // if true: return true
    this->_jump_manager.jump_to_label(instructions::jzero_label, true_label);
    // if false: return false
    this->_jump_manager.jump_to_label(instructions::jump_label, false_label);

    // 'if true' code will be inserted just below the true_label
    this->_jump_manager.insert_label(true_label);

    b_register.release();

    // 'if false' code will be inserted just below the true_label
    return condition::branch{false_label};
}

condition::branch code_builder::greater_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    const std::string true_label{this->_jump_manager.new_label("ge_true")};
    const std::string false_label{this->_jump_manager.new_label("ge_false")};

    auto& accumulator{this->_memory_manager.get_accumulator()};

    // if a == 0: return false
    this->initialize_identifier_value_in_register(a, accumulator);
    this->_jump_manager.jump_to_label(instructions::jzero_label, false_label);

    // if a > 0: check condition
    accumulator.acquire();
    auto& b_register{this->_memory_manager.acquire_free_register()};
    this->initialize_identifier_value_in_register(b, b_register);
    accumulator.release();

    // check a - b > 0
    this->add_instruction(instructions::sub(b_register));
    // if true: return true
    this->_jump_manager.jump_to_label(instructions::jpos_label, true_label);
    // if false: return false
    this->_jump_manager.jump_to_label(instructions::jump_label, false_label);

    // 'if true' code will be inserted just below the true_label
    this->_jump_manager.insert_label(true_label);

    b_register.release();

    // 'if false' code will be inserted just below the true_label
    return condition::branch{false_label};
}

condition::branch code_builder::greater_equal_condition(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    // a >= b <==> b <= a
    return this->less_equal_condition(b, a);
}

void code_builder::subtract(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    auto& accumulator{this->_memory_manager.get_accumulator()};

    const std::string end_label{this->_jump_manager.new_label("sub_end")};

    // load a
    this->initialize_identifier_value_in_register(a, accumulator);
    // if a == 0: return 0; (omit loading b if possible)
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // else: perform subtraction
    auto& b_register{this->_memory_manager.acquire_free_register()};
    this->initialize_identifier_value_in_register(b, b_register);
    this->add_instruction(instructions::sub(b_register));
    this->_jump_manager.jump_to_label(instructions::jump_label, end_label);

    // return
    this->_jump_manager.insert_label(end_label);

    b_register.release();
}

void code_builder::multiply(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    auto& is_odd_register{this->_memory_manager.acquire_free_register()};
    auto& result_register{this->_memory_manager.acquire_free_register()};

    const std::string end_label{this->_jump_manager.new_label("mul_end")};
    const std::string loop_begin_label{this->_jump_manager.new_label("mul_loop")};
    const std::string inside_loop_label{this->_jump_manager.new_label("mul_inside_loop")};
    const std::string is_odd_label{this->_jump_manager.new_label("mul_is_odd")};

    // set initial result to 0
    this->add_instruction(instructions::rst(result_register));

    // load a to acc
    this->initialize_identifier_value_in_register(a, accumulator);
    // check a == 0 -> if true: return 0
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // if false: put a
    this->add_instruction(instructions::put(a_register));

    // load b to designated register
    this->initialize_identifier_value_in_register(b, b_register);

    // begin multiplication loop
    this->_jump_manager.insert_label(loop_begin_label);

    // check b == 0 (initial and per iteration check)
    this->add_instruction(instructions::get(b_register));
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);

    this->add_instruction(instructions::put(is_odd_register)); // odd := b
    this->add_instruction(instructions::shr(is_odd_register)); // odd >> 1
    this->add_instruction(instructions::shl(is_odd_register)); // odd << 1 (get rid of last bit)
    this->add_instruction(instructions::sub(is_odd_register)); // b := b - odd
    this->_jump_manager.jump_to_label(instructions::jpos_label, is_odd_label);
    this->_jump_manager.insert_label(inside_loop_label);
    this->add_instruction(instructions::shl(a_register)); // a := a * 2
    this->add_instruction(instructions::shr(b_register)); // b := b / 2
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

    a_register.release();
    b_register.release();
    is_odd_register.release();
    result_register.release();
}

void code_builder::divide(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    auto& result_register{this->_memory_manager.acquire_free_register()};
    this->add_instruction(instructions::rst(result_register)); // result := 0

    const std::string end_label{this->_jump_manager.new_label("div_end")};
    const std::string loop_begin_label{this->_jump_manager.new_label("div_loop")};

    // load a to acc
    this->initialize_identifier_value_in_register(a, accumulator);
    // check a == 0 -> if true: return 0
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // if false: put a
    this->add_instruction(instructions::put(a_register));

    // load b to acc
    this->initialize_identifier_value_in_register(b, accumulator);
    // check b == 0 -> if true: return 0
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // if false: put b
    this->add_instruction(instructions::put(b_register));

    // begin division loop
    this->_jump_manager.insert_label(loop_begin_label);
    // auto& accumulator{this->_memory_manager.get_accumulator()};

    // check a < b
    this->add_instruction(instructions::get(b_register));
    this->add_instruction(instructions::sub(a_register));
    // if true: end loop
    this->_jump_manager.jump_to_label(instructions::jpos_label, end_label);
    // if false: result++ and a -= b
    // TODO: make this logarithmic
    this->add_instruction(instructions::inc(result_register));
    this->add_instruction(instructions::get(a_register));
    this->add_instruction(instructions::sub(b_register));
    this->add_instruction(instructions::put(a_register));
    this->_jump_manager.jump_to_label(instructions::jump_label, loop_begin_label);

    // end loop
    this->_jump_manager.insert_label(end_label);
    // return result (move to acc)
    this->add_instruction(instructions::get(result_register));

    a_register.release();
    b_register.release();
    result_register.release();
}

void code_builder::modulo(
    const std::shared_ptr<identifier::abstract_identifier>& a,
    const std::shared_ptr<identifier::abstract_identifier>& b
) {
    auto& accumulator{this->_memory_manager.get_accumulator()};
    auto& a_register{this->_memory_manager.acquire_free_register()};
    auto& b_register{this->_memory_manager.acquire_free_register()};

    auto& result_register{this->_memory_manager.acquire_free_register()};
    this->add_instruction(instructions::rst(result_register)); // result := 0

    const std::string end_label{this->_jump_manager.new_label("div_end")};
    const std::string loop_begin_label{this->_jump_manager.new_label("div_loop")};

    // load a to acc
    this->initialize_identifier_value_in_register(a, accumulator);
    // check a == 0 -> if true: return 0
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // if false: put a
    this->add_instruction(instructions::put(a_register));

    // load b to acc
    this->initialize_identifier_value_in_register(b, accumulator);
    // check b == 0 -> if true: return 0
    this->_jump_manager.jump_to_label(instructions::jzero_label, end_label);
    // if false: put b
    this->add_instruction(instructions::put(b_register));

    // begin division loop
    this->_jump_manager.insert_label(loop_begin_label);
    // auto& accumulator{this->_memory_manager.get_accumulator()};

    // check a < b
    this->add_instruction(instructions::get(b_register));
    this->add_instruction(instructions::sub(a_register));
    // if true: end loop
    this->_jump_manager.jump_to_label(instructions::jpos_label, end_label);
    // if false: and a -= b
    // TODO: make this logarithmic
    this->add_instruction(instructions::get(a_register));
    this->add_instruction(instructions::sub(b_register));
    this->add_instruction(instructions::put(a_register));
    this->add_instruction(instructions::put(result_register));
    this->_jump_manager.jump_to_label(instructions::jump_label, loop_begin_label);

    // end loop
    this->_jump_manager.insert_label(end_label);
    // return a (move to acc)
    this->add_instruction(instructions::get(result_register));

    a_register.release();
    b_register.release();
    result_register.release();
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

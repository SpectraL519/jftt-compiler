#include "assembly/jump_manager.hpp"
#include "assembly/code_builder.hpp"



namespace jftt::assembly {

jump_manager::jump_manager(code_builder& asm_builder)
: _asm_builder(asm_builder) {}

std::string jump_manager::new_label(const std::string& name) {
    return name + "_" + std::to_string(this->_label_no++);
}

std::string jump_manager::new_label() {
    return this->new_label(this->_default_label_name);
}

void jump_manager::insert_label(const std::string& label_name) {
    const auto line_no{this->_asm_builder.current_line()};

    auto label_it{this->_labels.find(label_name)};
    if (label_it == this->_labels.end()) {
        // there is no label with given name -> initialize with jump_to point
        this->_labels[label_name] = jump_label{line_no, std::vector<std::size_t>{}};
    }
    else {
        // line_no is the label's jump_to point
        label_it->second.jump_to = line_no;
    }
}

void jump_manager::jump_to_label(
    const std::string& jump_intsruction, const std::string& label_name
) {
    // add an incomplete jump instruction (without argument)
    // this will be filled in the fill_labels method
    this->_asm_builder.add_instruction(jump_intsruction);

    const auto line_no{this->_asm_builder.current_line()};

    auto label_it{this->_labels.find(label_name)};
    if (label_it == this->_labels.end()) {
        // label was not created -> initialize only jump_from point
        this->_labels[label_name] = jump_label{
            this->_invalid_jump_to, std::vector<std::size_t>{line_no}};
    }
    else {
        // add jump_from point to label
        label_it->second.jump_from_list.emplace_back(line_no);
    }
}

void jump_manager::fill_labels() {
    for (auto& label_it : this->_labels) {
        auto& label{label_it.second};
        for (const auto& jump_from : label.jump_from_list) {
            this->_asm_builder._code.at(jump_from) += std::to_string(label.jump_to);
        }
        label.jump_from_list.clear();
    }
}

} // namespace jftt::assembly

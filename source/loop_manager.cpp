#include "loop_manager.hpp"



namespace jftt {

void loop_manager::add_loop(std::shared_ptr<loop::abstract_loop> loop) {
    this->_loop_stack.push(std::move(loop));
}

const std::shared_ptr<loop::abstract_loop>& loop_manager::get_loop() const {
    return this->_loop_stack.top();
}

std::shared_ptr<loop::abstract_loop> loop_manager::extract_loop() {
    auto top_loop{this->_loop_stack.top()};
    this->_loop_stack.pop();
    return top_loop;
}

} // namespace jftt

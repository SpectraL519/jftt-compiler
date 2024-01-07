#pragma once

#include "loop.hpp"

#include <stack>



namespace jftt {

class loop_manager {
public:
    loop_manager() = default;
    ~loop_manager() = default;

    void add_loop(std::shared_ptr<loop::abstract_loop> loop) {
        this->_loop_stack.push(std::move(loop));
    }

    [[nodiscard]] const std::shared_ptr<loop::abstract_loop>& get_loop() const {
        return this->_loop_stack.top();
    }

    [[nodiscard]] std::shared_ptr<loop::abstract_loop> extract_loop() {
        auto top_loop{this->_loop_stack.top()};
        this->_loop_stack.pop();
        return top_loop;
    }

private:
    std::stack<std::shared_ptr<loop::abstract_loop>> _loop_stack;
};

} // namespace jftt

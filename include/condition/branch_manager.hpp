#pragma once

#include "branch.hpp"

#include <string>
#include <stack>



namespace jftt::condition {

class branch_manager {
public:
    branch_manager() = default;
    ~branch_manager() = default;

    void add_branch(const branch& branch) {
        this->_branch_stack.push(branch);
    }

    [[nodiscard]] branch extract_branch() {
        auto top_branch{this->_branch_stack.top()};
        this->_branch_stack.pop();
        return top_branch;
    }

private:
    std::stack<branch> _branch_stack;
};

} // namespace jftt::condition

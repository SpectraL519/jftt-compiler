#include "condition/branch_manager.hpp"



namespace jftt::condition {

void branch_manager::add_branch(const branch& branch) {
    this->_branch_stack.push(branch);
}

branch branch_manager::extract_branch() {
    auto top_branch{this->_branch_stack.top()};
    this->_branch_stack.pop();
    return top_branch;
}

bool branch_manager::has_branches() const {
    return !this->_branch_stack.empty();
}

} // namespace jftt::condition

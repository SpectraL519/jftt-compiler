#pragma once

#include "branch.hpp"

#include <stack>



namespace jftt::condition {

class branch_manager {
public:
    branch_manager() = default;
    ~branch_manager() = default;

    void add_branch(const branch& branch);
    [[nodiscard]] branch extract_branch();
    [[nodiscard]] bool has_branches() const;

private:
    std::stack<branch> _branch_stack;
};

} // namespace jftt::condition

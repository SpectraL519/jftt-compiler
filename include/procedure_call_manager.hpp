#pragma once

#include <string>
#include <stack>



namespace jftt {

class procedure_call_manager {
public:
    procedure_call_manager() = default;
    ~procedure_call_manager() = default;

    void add_procedure_call(const std::string& procedure_name) {
        this->_procedure_call_stack.push(procedure_name);
    }

    [[nodiscard]] bool has_procedure_calls() const {
        return !this->_procedure_call_stack.empty();
    }

    [[nodiscard]] const std::string& get_procedure() const {
        return this->_procedure_call_stack.top();
    }

    [[nodiscard]] std::string extract_procedure() {
        auto top_procedure_call{this->_procedure_call_stack.top()};
        this->_procedure_call_stack.pop();
        return top_procedure_call;
    }

private:
    std::stack<std::string> _procedure_call_stack;
};

} // namespace jftt

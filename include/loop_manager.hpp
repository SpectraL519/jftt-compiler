#pragma once

#include "loop.hpp"

#include <memory>
#include <stack>



namespace jftt {

class loop_manager {
public:
    loop_manager() = default;
    ~loop_manager() = default;

    void add_loop(std::shared_ptr<loop::abstract_loop> loop);
    [[nodiscard]] const std::shared_ptr<loop::abstract_loop>& get_loop() const;
    [[nodiscard]] std::shared_ptr<loop::abstract_loop> extract_loop();

private:
    std::stack<std::shared_ptr<loop::abstract_loop>> _loop_stack;
};

} // namespace jftt

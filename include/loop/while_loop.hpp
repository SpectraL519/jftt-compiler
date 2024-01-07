#pragma once

#include "abstract_loop.hpp"



namespace jftt::loop {

class while_loop : public abstract_loop {
public:
    while_loop(
        const std::string& begin_label,
        const std::string& end_label
    ) : abstract_loop(type_discriminator::while_do, begin_label, end_label) {}

    while_loop(const while_loop&) = default;
    while_loop(while_loop&&) = default;

    while_loop& operator=(const while_loop&) = default;
    while_loop& operator=(while_loop&&) = default;

    ~while_loop() = default;
};

} // namespace jftt::loop

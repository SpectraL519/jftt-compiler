#pragma once

#include "abstract_loop.hpp"



namespace jftt::loop {

class repeat_until_loop : public abstract_loop {
public:
    repeat_until_loop(
        const std::string& begin_label,
        const std::string& end_label
    ) : abstract_loop(type_discriminator::repeat_until, begin_label, end_label) {}

    repeat_until_loop(const repeat_until_loop&) = default;
    repeat_until_loop(repeat_until_loop&&) = default;

    repeat_until_loop& operator=(const repeat_until_loop&) = default;
    repeat_until_loop& operator=(repeat_until_loop&&) = default;

    ~repeat_until_loop() = default;
};

} // namespace jftt::loop

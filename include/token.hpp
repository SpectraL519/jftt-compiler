#pragma once

#include <cstdint>
#include <string>



namespace jftt {

struct token {
    token() = default;
    token(const token&) = default;
    token(token&&) = default;
    token& operator= (const token&) = default;
    token& operator= (token&&) = default;

    ~token() = default;

    uint64_t line_no;
    uint64_t value;
    std::string* str;
};

} // namespace jftt
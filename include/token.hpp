#pragma once

#include <cstdint>
#include <string>



namespace jftt {

enum class token_discriminator { std, value, identifier };

struct token {
    token() = default;
    token(const token&) = default;
    token(token&&) = default;
    token& operator= (const token&) = default;
    token& operator= (token&&) = default;

    ~token() = default;

    token_discriminator discriminator;
    uint64_t line_no;
    uint64_t value;
    std::string* str_ptr;
};

} // namespace jftt
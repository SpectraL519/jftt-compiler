#pragma once

#include "architecture/vm_params.hpp"

#include <string>



namespace jftt::parser {

enum class token_discriminator : uint8_t { normal, value, identifier };

struct token {
    token() = default;
    token(const token&) = default;
    token(token&&) = default;
    token& operator= (const token&) = default;
    token& operator= (token&&) = default;

    ~token() = default;

    token_discriminator discriminator;
    std::size_t line_no;
    architecture::value_type value;
    std::string* str_ptr;
};

} // namespace jftt::parser

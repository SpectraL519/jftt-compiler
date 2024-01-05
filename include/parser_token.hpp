#pragma once

#include "architecture/vm_params.hpp"

#include <string>



namespace jftt {

enum class parser_token_discriminator : uint8_t {
    normal, rvalue, identifier
};

struct parser_token {


    parser_token() = default;
    parser_token(const parser_token&) = default;
    parser_token(parser_token&&) = default;
    parser_token& operator= (const parser_token&) = default;
    parser_token& operator= (parser_token&&) = default;

    ~parser_token() = default;

    parser_token_discriminator discriminator;
    std::size_t line_no;
    architecture::value_type value;
    std::string* str_ptr;
};

} // namespace jftt

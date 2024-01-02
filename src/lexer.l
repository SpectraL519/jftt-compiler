%option noyywrap

%{
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "parser.hpp"

#include <iostream>


// utility
static uint64_t program_line_no{1};

// debugging
#define LEXER_DEBUG
static std::string lexer_debug_info(const uint64_t line_no, const std::string& msg);

#ifdef LEXER_DEBUG
#endif

%}


/* patterns */
p_whitespace [ \t]
p_newline (\r\n)|\n
p_comment ^#.*$


%%

{p_whitespace}+ {
#ifdef LEXER_DEBUG
    std::cout << lexer_debug_info(program_line_no, "[pattern = whitespace]") << std::endl;
#endif
}

p_newline {
#ifdef LEXER_DEBUG
    std::cout << lexer_debug_info(program_line_no, "[pattern = newline]") << std::endl;
#endif
    program_line_no++;
}

p_comment {
#ifdef LEXER_DEBUG
    std::cout << lexer_debug_info(program_line_no, "[pattern = comment] ") 
              << yytext << std::endl;
#endif
}

%%




/* function definitions */

static std::string lexer_debug_info(const uint64_t line_no, const std::string& msg) {
    return "[DEBUG = lexer] : [line = " + std::to_string(line_no) + "] : " + msg;
}


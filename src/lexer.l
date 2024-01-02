%option noyywrap

%{
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "token.hpp"
#include "parser.hpp"

#include <iostream>


// utility
uint64_t program_line_no{1};
static constexpr uint64_t default_token_line_no{0};
static constexpr uint64_t default_token_value{0};

// static int std_token(int token);
// static int identifier_token(int token);
// static int value_token(int token);


// debugging
#define DEBUG_LEXER
// #define LEXER_DEBUG_ALL
static void print_lexer_debug_info(const uint64_t, const std::string&, const std::string&);

#ifdef DEBUG_LEXER
#endif

%}


/* patterns */
p_newline (\r\n)|\n
p_whitespace [ \t]
p_comment \#.*$
p_identifier [_a-z]+
p_number [0-9]+


%%

{p_whitespace}+ {
#ifdef LEXER_DEBUG_ALL
    print_lexer_debug_info(program_line_no, "pattern = whitespace", yytext);
#else
    ECHO;
#endif
}

{p_newline} {
#ifdef LEXER_DEBUG_ALL
    print_lexer_debug_info(program_line_no, "pattern = newline", yytext);
#else
    ECHO;
#endif
    program_line_no++;
}

{p_comment} {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "pattern = comment", yytext);
#else
    ECHO;
#endif
}

. { ECHO; }

%%




/* function definitions */

// int std_token(int token) {
//     yylval.token.discriminator = jftt::token_discriminator::std;
//     yylval.token.line_no = program_line_no;
//     return token;
// }
// 
// int identifier_token(int token) {
//     yylval.token.discriminator = jftt::token_discriminator::identifier;
//     yylval.token.line_no = program_line_no;
//     yylval.token.str_ptr = new std::string(yytext);
//     return token;
// }
// 
// int value_token(int token) {
//     yylval.token.discriminator = jftt::token_discriminator::value;
//     yylval.token.line_no = program_line_no;
//     yylval.token.value = std::stoull(yytext);
//     return token;
// }

void print_lexer_debug_info(
    const uint64_t line_no, const std::string& description, const std::string& value
) {
    std::cout << "[DEBUG = lexer] : [line = " << line_no << "] : [" << description
              << "]\n\tvalue = `" << value << '`' << std::endl;
}


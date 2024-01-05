%option noyywrap

%{
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "parser.hpp"
#include "parser_token.hpp"

using jftt::parser_token, jftt::parser_token_discriminator;

#include <iostream>


// utility
uint64_t program_line_no{1};
static constexpr uint64_t default_token_line_no{0};
static constexpr uint64_t default_token_value{0};

static int normal_token(int token);
static int identifier_token(int token);
static int value_token(int token);


// debugging
// #define DEBUG_LEXER
static void print_lexer_debug_info(const std::size_t, const std::string&, const std::string&);

%}


/* patterns */
p_newline (\r\n)|\n
p_whitespace [ \t]
p_comment \#.*$
p_identifier [_a-z]+
p_number [0-9]+


%%

{p_whitespace}+ {}
{p_newline}     { program_line_no++; }

{p_comment} {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "pattern = comment", yytext);
#endif
}


PROGRAM {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_PROGRAM", yytext);
#endif
    return normal_token(T_PROGRAM);
}

PROCEDURE {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_PROCEDURE", yytext);
#endif
    return normal_token(T_PROCEDURE);
}

IS {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_IS", yytext);
#endif
    return normal_token(T_IS);
}

IN {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_IN", yytext);
#endif
    return normal_token(T_IN);
}

END {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_END", yytext);
#endif
    return normal_token(T_END);
}


IF {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_IF", yytext);
#endif
    return normal_token(T_IF);
}

THEN {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_THEN", yytext);
#endif
    return normal_token(T_THEN);
}

ELSE {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_ELSE", yytext);
#endif
    return normal_token(T_ELSE);
}

ENDIF {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_ENDIF", yytext);
#endif
    return normal_token(T_ENDIF);
}


WHILE {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_WHILE", yytext);
#endif
    return normal_token(T_WHILE);
}

DO {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_DO", yytext);
#endif
    return normal_token(T_DO);
}

ENDWHILE {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_ENDWHILE", yytext);
#endif
    return normal_token(T_ENDWHILE);
}


REPEAT {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_REPEAT", yytext);
#endif
    return normal_token(T_REPEAT);
}

UNTIL {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_UNTIL", yytext);
#endif
    return normal_token(T_UNTIL);
}


READ {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_READ", yytext);
#endif
    return normal_token(T_READ);
}

WRITE {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_WRITE", yytext);
#endif
    return normal_token(T_WRITE);
}


\( {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_LPAREN", yytext);
#endif
    return normal_token(T_LPAREN);
}

\) {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_RPAREN", yytext);
#endif
    return normal_token(T_RPAREN);
}

\[ {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_LBRACKET", yytext);
#endif
    return normal_token(T_LBRACKET);
}

\] {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_RBRACKET", yytext);
#endif
    return normal_token(T_RBRACKET);
}

\:\= {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_ASSIGN", yytext);
#endif
    return normal_token(T_ASSIGN);
}

\; {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_SEMICOLON", yytext);
#endif
    return normal_token(T_SEMICOLON);
}

\, {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_COMMA", yytext);
#endif
    return normal_token(T_COMMA);
}


\= {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_EQ", yytext);
#endif
    return normal_token(T_EQ);
}

\!\= {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_NEQ", yytext);
#endif
    return normal_token(T_NEQ);
}

\< {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_LE", yytext);
#endif
    return normal_token(T_LE);
}

\<\= {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_LEQ", yytext);
#endif
    return normal_token(T_LEQ);
}

\> {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_GE", yytext);
#endif
    return normal_token(T_GE);
}

\>\= {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_GEQ", yytext);
#endif
    return normal_token(T_GEQ);
}

\+ {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_ADD", yytext);
#endif
    return normal_token(T_ADD);
}

\- {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_SUB", yytext);
#endif
    return normal_token(T_SUB);
}

\* {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_MUL", yytext);
#endif
    return normal_token(T_MUL);
}

\/ {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_DIV", yytext);
#endif
    return normal_token(T_DIV);
}

\% {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_MOD", yytext);
#endif
    return normal_token(T_MOD);
}


{p_identifier} {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_IDENTIFIER", yytext);
#endif
    return identifier_token(T_IDENTIFIER);
}

{p_number} {
#ifdef DEBUG_LEXER
    print_lexer_debug_info(program_line_no, "token = T_NUMBER", yytext);
#endif
    return value_token(T_NUMBER);
}


. { yyerror("[SYNTAX ERROR] : [line = TODO] \n\t msg = TODO"); }

%%




/* function definitions */

int normal_token(int token) {
    yylval.token.discriminator = parser_token_discriminator::normal;
    yylval.token.line_no = program_line_no;
    return token;
}

int identifier_token(int token) {
    yylval.token.discriminator = parser_token_discriminator::identifier;
    yylval.token.line_no = program_line_no;
    yylval.token.str_ptr = new std::string(yytext);
    return token;
}

int value_token(int token) {
    yylval.token.discriminator = parser_token_discriminator::rvalue;
    yylval.token.line_no = program_line_no;
    yylval.token.value = std::stoull(yytext);
    return token;
}

void print_lexer_debug_info(
    const std::size_t line_no, const std::string& description, const std::string& value
) {
    std::cout << "[DEBUG = lexer | line = " << line_no << " | " << description
              << "]\n\tvalue = `" << value << '`' << std::endl;
}

%code {

#include "io.hpp"
#include "token.hpp"

#include <iostream>

// Flex & Bison utility
int yylex();
int yyparse();
int yyerror(const char* msg);

// debugging
#define PARSER_DEBUG

}


%code requires {

#include "token.hpp"

}


/* yylval type */
%union {
    jftt::token token;
    int value; // TODO
}


/* terminal symbols - keywords */
%token <token> T_PROGRAM T_PROCEDURE T_IS T_IN T_END
%token <token> T_IF T_THEN T_ELSE T_ENDIF
%token <token> T_WHILE T_DO T_ENDWHILE
%token <token> T_REPEAT T_UNTIL
%token <token> T_READ T_WRITE

/* terminal symbols - special characters */
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET
%token <token> T_ASSIGN T_SEMICOLON T_COMMA

/* terminal symbols - arithmetics */
%token <token> T_EQ T_NEQ T_LE T_LEQ T_GE T_GEQ
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD

/* terminal symbols - identifiers & values */
%token <token> T_IDENTIFIER T_NUMBER 

/* terminal symbols - error */
%token <token> T_ERROR


/* non-terminal symbols */
%type <value> value lvalue rvalue


/* grammar */
%%

program_all:
    procedures main {
        // TODO
    }
    ;


procedures:
    procedures T_PROCEDURE procedure_head T_IS declarations T_IN commands T_END {
        // TODO
    }
    |
    procedures T_PROCEDURE procedure_head T_IS T_IN commands T_END {
        // TODO
    }
    |
    /* empty production */
    ;


main:
    T_PROGRAM T_IS declarations T_IN commands T_END {
        // TODO
    }
    |
    T_PROGRAM T_IS T_IN commands T_END {
        // TODO
    }
    ;


commands:
    commands command {
        // TODO
    }
    |
    command {
        // TODO
    }
    ;


command:
    lvalue T_ASSIGN expression T_SEMICOLON {
        // TODO
    }
    |
    T_IF condition T_THEN commands T_ELSE commands T_ENDIF {
        // TODO
    }
    |
    T_IF condition T_THEN commands T_ENDIF {
        // TODO
    }
    |
    T_WHILE condition T_DO commands T_ENDWHILE {
        // TODO
    }
    |
    T_REPEAT commands T_UNTIL condition T_SEMICOLON {
        // TODO
    }
    |
    procedure_call T_SEMICOLON {
        // TODO
    }
    |
    T_READ lvalue T_SEMICOLON {
        // TODO
    }
    |
    T_WRITE value T_SEMICOLON {
        // TODO
    }
    ;


procedure_head:
    lvalue T_LPAREN procedure_args_decl T_RPAREN {
        // TODO
    }
    ;


procedure_call:
    lvalue T_LPAREN procedure_args T_RPAREN {

    }
    ;


declarations:
    declarations T_SEMICOLON T_IDENTIFIER {
        // TODO
    }
    |
    declarations T_SEMICOLON T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        // TODO
    }
    |
    T_IDENTIFIER {
        // TODO
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        // TODO
    }
    ;


procedure_args_decl:
    procedure_args_decl T_COMMA lvalue {
        // TODO
    }
    |
    procedure_args_decl T_COMMA "T" lvalue {
        // TODO
    }
    |
    lvalue {
        // TODO
    }
    |
    "T" lvalue {
        // TODO
    }
    ;


procedure_args:
    procedure_args T_COMMA value {
        // TODO
    }
    |
    value {
        // TODO
    }
    ;


expression:
    value {
        // TODO
    }
    |
    value T_ADD value {
        // TODO
    }
    |
    value T_SUB value {
        // TODO
    }
    |
    value T_MUL value {
        // TODO
    }
    |
    value T_DIV value {
        // TODO
    }
    |
    value T_MOD value {
        // TODO
    }
    ;


condition:
    value T_EQ value {
        // TODO
    }
    |
    value T_NEQ value {
        // TODO
    }
    |
    value T_LE value {
        // TODO
    }
    |
    value T_LEQ value {
        // TODO
    }
    |
    value T_GE value {
        // TODO
    }
    |
    value T_GEQ value {
        // TODO
    }
    ;


value:
    lvalue {
        // TODO
    }
    |
    rvalue {
        // TODO
    }
    ;


lvalue:
    T_IDENTIFIER {
        // TODO
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        // TODO
    }
    |
    T_IDENTIFIER T_LBRACKET T_IDENTIFIER T_RBRACKET {
        // TODO
    }
    ;


rvalue:
    T_NUMBER {
        // TODO
    }
    ;

%%


int yyerror(const char* msg) {
    constexpr int todo_line_in_yylval = 0;
    std::cerr << "[ERROR] : " << msg << "\n\tIn line: " << todo_line_in_yylval << std::endl;
    return 1;
}

int compile(const std::string& infile, const std::string& outfile) {
    std::cout << infile << " " << outfile << std::endl;
    return 0;

    // const auto yy_str_buffer = yy_scan_string(jftt::io::read(infile));
    // const int yy_result = yyparse();
    // yy_delete_buffer(yy_str_buffer);

    // TODO: io::write_lines(code, outfile)
}

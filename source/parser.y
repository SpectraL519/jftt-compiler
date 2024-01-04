%code {

#include "io.hpp"
#include "compiler.hpp"

#include <iostream>

// utility
jftt::compiler compiler;

// Flex & Bison utility
int yylex();
int yyparse();
extern FILE *yyin;

// debugging
#define DEBUG_PARSER

}


%code requires {

#include "parser/token.hpp"
#include "parser/values.hpp"

namespace parser = jftt::parser;

// Flex & Bison utility
int yyerror(const char* msg);

}


/* yylval type */
%union {
    jftt::parser::token token;
    jftt::parser::abstract_value_base* value;
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
%type <value> value identifier


/* grammar */
%%

program_all:
    procedures main {
        compiler.finish_code_generating();
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
    identifier T_ASSIGN expression T_SEMICOLON {
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
    T_READ identifier T_SEMICOLON {
        // TODO
    }
    |
    T_WRITE value T_SEMICOLON {
        parser::rvalue* rvalue{nullptr};
        // parser::identifier* identifier{nullptr};

        switch ($2->discriminator()) {
            case parser::value_discriminator::rvalue:
                rvalue = parser::as_rvalue($2);
                compiler.print_value(rvalue->value());
                delete rvalue;
                break;

            case parser::value_discriminator::identifier:
                // TODO;
                break;
        }
    }
    ;


procedure_head:
    identifier T_LPAREN procedure_args_decl T_RPAREN {
        // TODO
    }
    ;


procedure_call:
    identifier T_LPAREN procedure_args T_RPAREN {

    }
    ;


declarations:
    declarations T_COMMA T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        // TODO
    }
    |
    declarations T_COMMA T_IDENTIFIER {
        // TODO
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        // TODO
    }
    |
    T_IDENTIFIER {
        // TODO
    }
    ;


procedure_args_decl:
    procedure_args_decl T_COMMA identifier {
        // TODO
    }
    |
    procedure_args_decl T_COMMA "T" identifier {
        // TODO
    }
    |
    identifier {
        // TODO
    }
    |
    "T" identifier {
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
    T_NUMBER {
        $$ = new parser::rvalue($1.value);
    }
    |
    identifier {
        // TODO
    }
    ;


identifier:
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

%%


int yyerror(const char* msg) {
    std::cerr << "[ERROR] : [line = " << yylval.token.line_no << "] " << msg << std::endl;
    return 1;
}

int compile(const std::string& infile, const std::string& outfile) {
    yyin = fopen(infile.c_str(), "r");
    if (yyin == NULL) {
        std::cerr << "[ERROR] Failed to open file: " << infile << std::endl;
        std::exit(1);
    }

    const int yyresult = yyparse();
    fclose(yyin);

    const auto& code{compiler.asm_code()};
    jftt::io::write_lines(code, outfile);

    return yyresult;
}

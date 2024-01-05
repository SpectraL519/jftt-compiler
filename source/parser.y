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

#include "identifier.hpp"
#include "parser/token.hpp"

// utility definitions
namespace id = jftt::identifier;
namespace parser = jftt::parser;

void assert_identifier_token(const parser::token_discriminator);
void assert_rvalue_token(const parser::token_discriminator);

// Flex & Bison utility
int yyerror(const char* msg);

}


/* yylval type */
%union {
    parser::token token;
    id::abstract_identifier* identifier;
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
%type <identifier> value identifier


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
        switch ($2->discriminator()) {
            case id::type_discriminator::variable:
                std::cout << "scanning: " << $2->name() << std::endl;
                compiler.scan($2);
                break;

            default:
                break; // TODO
        }
        delete $2;
    }
    |
    T_WRITE value T_SEMICOLON {
        compiler.print($2);
        delete $2;
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
        compiler.set_line_no($3.line_no);
        compiler.declare_vararray(*$3.str_ptr, $5.value);
    }
    |
    declarations T_COMMA T_IDENTIFIER {
        compiler.set_line_no($3.line_no);
        compiler.declare_variable(*$3.str_ptr);
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        compiler.set_line_no($1.line_no);
        compiler.declare_vararray(*$3.str_ptr, $3.value);
    }
    |
    T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.declare_variable(*$1.str_ptr);
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
        assert_rvalue_token($1.discriminator);
        $$ = new id::rvalue($1.value);
    }
    |
    identifier {
        $$ = $1;
    }
    ;


identifier:
    T_IDENTIFIER {
        assert_identifier_token($1.discriminator);
        $$ = new id::variable(*id::raw_ptr_cast<id::type_discriminator::variable>(
                                compiler.get_identifier(*$1.str_ptr)));
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


void assert_identifier_token(const parser::token_discriminator discriminator) {
    if (discriminator == parser::token_discriminator::identifier)
        return;

    std::cerr << "[ERROR] : Cannot initialize an identifier from a non-identifier token" << std::endl;
    std::exit(1);
}

void assert_rvalue_token(const parser::token_discriminator discriminator) {
    if (discriminator == parser::token_discriminator::rvalue)
        return;

    std::cerr << "[ERROR] : Cannot initialize an rvalue from a non-rvalue token" << std::endl;
    std::exit(1);
}

void assert_rvalue_token(const parser::token_discriminator);

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

%code {

#include "io.hpp"
#include "compiler.hpp"

#include <iostream>
#include <optional>
#include <vector>

// utility
jftt::compiler compiler;
std::optional<std::string> current_procedure;
std::optional<std::string> current_procedure_call;
const std::vector<id::type_discriminator> var_discriminators_list{
    id::type_discriminator::variable, id::type_discriminator::vararray};

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

// utility
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
%token <token> T_PROGRAM T_PROCEDURE T_IS T_IN T_END T_PROCEDURE_T
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
    procedures procedure_decl T_LPAREN procedure_params_decl T_RPAREN T_IS declarations procedure_begin commands procedure_end {}
    |
    procedures procedure_decl T_LPAREN procedure_params_decl T_RPAREN T_IS procedure_begin commands procedure_end {}
    |
    /* empty production */
    ;


main:
    program_begin declarations program_content {}
    |
    program_begin program_content {}
    ;

program_begin:
    T_PROGRAM T_IS {
        compiler.set_line_no($1.line_no);
        compiler.begin_program();
    }

program_content:
    T_IN commands T_END {}
    ;


commands:
    commands command {}
    |
    command {}
    ;


command:
    identifier T_ASSIGN expression T_SEMICOLON {
        compiler.set_line_no($2.line_no);
        compiler.assign_value_to($1, current_procedure);
        compiler.release_accumulator();
    }
    |
    if_condition if_else if_end {}
    |
    if_condition if_end {}
    |
    while_loop while_loop_end {}
    |
    repeat_loop repeat_loop_end {}
    |
    procedure_call T_SEMICOLON {}
    |
    T_READ identifier T_SEMICOLON {
        compiler.set_line_no($1.line_no);
        compiler.scan($2, current_procedure);
    }
    |
    T_WRITE value T_SEMICOLON {
        compiler.set_line_no($1.line_no);
        compiler.print($2, current_procedure);
    }
    ;


procedure_decl:
    T_PROCEDURE T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.with_procedurers();
        compiler.declare_procedure(*$2.str_ptr);
        current_procedure.emplace(*$2.str_ptr);
        delete $2.str_ptr;
    }
    ;

procedure_params_decl:
    procedure_params_decl T_COMMA T_IDENTIFIER {
        compiler.set_line_no($3.line_no);
        compiler.declare_procedure_parameter(
            current_procedure.value(), id::type_discriminator::variable, *$3.str_ptr);
        delete $3.str_ptr;
    }
    |
    procedure_params_decl T_COMMA T_PROCEDURE_T T_IDENTIFIER {
        compiler.set_line_no($3.line_no);
        compiler.declare_procedure_parameter(
            current_procedure.value(), id::type_discriminator::vararray, *$4.str_ptr);
        delete $4.str_ptr;
    }
    |
    T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.declare_procedure_parameter(
            current_procedure.value(), id::type_discriminator::variable, *$1.str_ptr);
        delete $1.str_ptr;
    }
    |
    T_PROCEDURE_T T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.declare_procedure_parameter(
            current_procedure.value(), id::type_discriminator::vararray, *$2.str_ptr);
        delete $2.str_ptr;
    }
    |
    /* emmpty production */
    ;

procedure_begin:
    T_IN {
        compiler.set_line_no($1.line_no);
        compiler.begin_procedure_implementation(current_procedure.value());
    }
    ;

procedure_end:
    T_END {
        compiler.set_line_no($1.line_no);
        compiler.return_from_procedure(current_procedure.value());
        current_procedure = std::nullopt;
    }
    ;


declarations:
    declarations T_COMMA T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        compiler.set_line_no($3.line_no);
        compiler.declare_vararray(*$3.str_ptr, $5.value, current_procedure);
        delete $3.str_ptr;
    }
    |
    declarations T_COMMA T_IDENTIFIER {
        compiler.set_line_no($3.line_no);
        compiler.declare_variable(*$3.str_ptr, current_procedure);
        delete $3.str_ptr;
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        compiler.set_line_no($1.line_no);
        compiler.declare_vararray(*$1.str_ptr, $3.value, current_procedure);
        delete $3.str_ptr;
    }
    |
    T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.declare_variable(*$1.str_ptr, current_procedure);
        delete $1.str_ptr;
    }
    ;


procedure_call:
    procedure_call_begin T_LPAREN procedure_args T_RPAREN {
        compiler.set_line_no($2.line_no);
        compiler.end_procedure_call_args_declaration(current_procedure_call.value());
        compiler.call_procedure(current_procedure_call.value());
        current_procedure_call = std::nullopt;
    }
    ;

procedure_call_begin:
    T_IDENTIFIER {
        compiler.set_line_no($1.line_no);
        compiler.assert_identifier_defined(
            *$1.str_ptr, id::type_discriminator::procedure);

        if (current_procedure && *$1.str_ptr == current_procedure.value())
            compiler.throw_error("Recursive procedure call: " + *$1.str_ptr);

        current_procedure_call.emplace(*$1.str_ptr);
        delete $1.str_ptr;
    }
    ;

procedure_args:
    procedure_args T_COMMA identifier {
        compiler.set_line_no($2.line_no);
        compiler.pass_procedure_parameter(current_procedure_call.value(), $3, current_procedure);
    }
    |
    identifier {
        compiler.pass_procedure_parameter(current_procedure_call.value(), $1, current_procedure);
    }
    |
    /* empty production */
    ;


expression:
    value {
        compiler.acquire_accumulator();
        compiler.return_value($1, current_procedure);
    }
    |
    value T_ADD value {
        compiler.set_line_no($2.line_no);
        compiler.acquire_accumulator();
        compiler.add($1, $3, current_procedure);
    }
    |
    value T_SUB value {
        compiler.set_line_no($2.line_no);
        compiler.acquire_accumulator();
        compiler.subtract($1, $3, current_procedure);
    }
    |
    value T_MUL value {
        compiler.set_line_no($2.line_no);
        compiler.acquire_accumulator();
        compiler.multiply($1, $3, current_procedure);
    }
    |
    value T_DIV value {
        compiler.set_line_no($2.line_no);
        compiler.acquire_accumulator();
        compiler.divide($1, $3, current_procedure);
    }
    |
    value T_MOD value {
        compiler.set_line_no($2.line_no);
        compiler.acquire_accumulator();
        compiler.modulo($1, $3, current_procedure);
    }
    ;


if_condition:
    T_IF condition T_THEN {
        compiler.set_line_no($1.line_no);
    }
    ;

if_else:
    commands T_ELSE {
        compiler.set_line_no($2.line_no);
        compiler.end_latest_condition_with_else();
    }

if_end:
    commands T_ENDIF {
        compiler.set_line_no($2.line_no);
        compiler.end_latest_condition_without_else();
    }


while_loop:
    while_loop_begin condition T_DO {
        compiler.set_line_no($3.line_no);
        compiler.set_latest_loop_end_label();
    }

while_loop_begin:
    T_WHILE {
        compiler.set_line_no($1.line_no);
        compiler.add_loop(jftt::loop_discriminator::while_do);
    }
    ;

while_loop_end:
    commands T_ENDWHILE {
        compiler.set_line_no($2.line_no);
        compiler.end_loop(jftt::loop_discriminator::while_do);
    }
    ;


repeat_loop:
    T_REPEAT {
        compiler.set_line_no($1.line_no);
        compiler.add_loop(jftt::loop_discriminator::repeat_until);
    }
    ;

repeat_loop_end:
    commands T_UNTIL condition T_SEMICOLON {
        compiler.set_line_no($2.line_no);
        compiler.set_latest_loop_end_label();
        compiler.end_loop(jftt::loop_discriminator::repeat_until);
    }
    ;


condition:
    value T_EQ value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::eq, $1, $3, current_procedure);
    }
    |
    value T_NEQ value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::neq, $1, $3, current_procedure);
    }
    |
    value T_LE value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::le, $1, $3, current_procedure);
    }
    |
    value T_LEQ value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::leq, $1, $3, current_procedure);
    }
    |
    value T_GE value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::ge, $1, $3, current_procedure);
    }
    |
    value T_GEQ value {
        compiler.set_line_no($2.line_no);
        compiler.add_condition(jftt::condition_discriminator::geq, $1, $3, current_procedure);
    }
    ;


value:
    T_NUMBER {
        compiler.set_line_no($1.line_no);
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
        compiler.set_line_no($1.line_no);
        assert_identifier_token($1.discriminator);

        if (current_procedure_call)
            compiler.assert_identifier_defined(
                *$1.str_ptr, var_discriminators_list, current_procedure);
        else
            compiler.assert_identifier_defined(
                *$1.str_ptr, id::type_discriminator::variable, current_procedure);


        auto identifier{compiler.get_identifier(*$1.str_ptr, current_procedure)};
        if (identifier->discriminator() == id::type_discriminator::reference)
            $$ = new id::reference(
                *id::shared_ptr_cast<id::type_discriminator::reference>(identifier));
        else if (identifier->discriminator() == id::type_discriminator::variable)
            $$ = new id::variable(
                *id::shared_ptr_cast<id::type_discriminator::variable>(identifier));
        else
            $$ = new id::vararray(
                *id::shared_ptr_cast<id::type_discriminator::vararray>(identifier));

        delete $1.str_ptr;
    }
    |
    T_IDENTIFIER T_LBRACKET T_NUMBER T_RBRACKET {
        compiler.set_line_no($1.line_no);
        assert_identifier_token($1.discriminator);
        assert_rvalue_token($3.discriminator);

        compiler.assert_index_in_range(*$1.str_ptr, $3.value, current_procedure);

        auto identifier{compiler.get_identifier(*$1.str_ptr, current_procedure)};
        if (identifier->discriminator() == id::type_discriminator::reference) {
            auto reference_ptr{new id::reference(
                *id::shared_ptr_cast<id::type_discriminator::reference>(identifier))};
            reference_ptr->set_indexer(std::make_shared<id::rvalue>($3.value));
            $$ = reference_ptr;
        }
        else {
            auto varrarray_ptr{new id::vararray(
                *id::shared_ptr_cast<id::type_discriminator::vararray>(identifier))};
            varrarray_ptr->set_indexer(std::make_shared<id::rvalue>($3.value));
            $$ = varrarray_ptr;
        }

        delete $1.str_ptr;
    }
    |
    T_IDENTIFIER T_LBRACKET T_IDENTIFIER T_RBRACKET {
        compiler.set_line_no($1.line_no);
        assert_identifier_token($1.discriminator);
        assert_identifier_token($3.discriminator);

        compiler.assert_identifier_defined(
            *$1.str_ptr, id::type_discriminator::vararray, current_procedure);
        compiler.assert_identifier_defined(
            *$3.str_ptr, id::type_discriminator::variable, current_procedure);
        compiler.assert_lvalue_initialized(
            *$3.str_ptr, id::type_discriminator::variable, current_procedure);

        auto identifier{compiler.get_identifier(*$1.str_ptr, current_procedure)};
        if (identifier->discriminator() == id::type_discriminator::reference) {
            auto reference_ptr{new id::reference(
                *id::shared_ptr_cast<id::type_discriminator::reference>(identifier))};

            auto indexer{compiler.get_identifier(*$3.str_ptr, current_procedure)};
            if (indexer->discriminator() == id::type_discriminator::variable)
                reference_ptr->set_indexer(
                    id::shared_ptr_cast<id::type_discriminator::variable>(indexer));
            else // reference
                reference_ptr->set_indexer(
                    id::shared_ptr_cast<id::type_discriminator::reference>(indexer));

            $$ = reference_ptr;
        }
        else {
            auto vararray_ptr{new id::vararray(
                *id::shared_ptr_cast<id::type_discriminator::vararray>(identifier))};

            auto indexer{compiler.get_identifier(*$3.str_ptr, current_procedure)};
            if (indexer->discriminator() == id::type_discriminator::variable)
                vararray_ptr->set_indexer(
                    id::shared_ptr_cast<id::type_discriminator::variable>(indexer));
            else // reference
                vararray_ptr->set_indexer(
                    id::shared_ptr_cast<id::type_discriminator::reference>(indexer));

            $$ = vararray_ptr;
        }

        delete $1.str_ptr;
        delete $3.str_ptr;
    }
    ;

%%


int yyerror(const char* msg) {
    std::cerr << msg << std::endl;
    std::exit(EXIT_FAILURE);
}


void assert_identifier_token(const parser::token_discriminator discriminator) {
    if (discriminator == parser::token_discriminator::identifier)
        return;

    std::cerr << "[ERROR] : Cannot initialize an identifier from a non-identifier token" << std::endl;
    std::exit(EXIT_FAILURE);
}

void assert_rvalue_token(const parser::token_discriminator discriminator) {
    if (discriminator == parser::token_discriminator::rvalue)
        return;

    std::cerr << "[ERROR] : Cannot initialize an rvalue from a non-rvalue token" << std::endl;
    std::exit(EXIT_FAILURE);
}

int compile(const std::string& infile, const std::string& outfile) {
    yyin = fopen(infile.c_str(), "r");
    if (yyin == NULL) {
        std::cerr << "[ERROR] Failed to open file: " << infile << std::endl;
        std::exit(EXIT_FAILURE);
    }

    const int yyresult = yyparse();
    fclose(yyin);

    const auto& code{compiler.asm_code()};
    jftt::io::write_lines(code, outfile);

    return yyresult;
}

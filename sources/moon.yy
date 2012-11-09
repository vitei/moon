/*
 * moon.yy
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include <stdio.h>
    #include "error.h"
    #include "tree.h"

    /* Generated headers */
    #include "parser.h"
    #include "lexer.h"

    void yyerror(YYLTYPE *locp, void *scanner, const char *error);
%}

/* Re-entrant */
%pure-parser
%lex-param {void *scanner}
%parse-param {void *scanner}

/* Program locations */
%locations

/* Better errors!! */
%error-verbose

/*  */
%union {
    tree::Expression *expression;
    tree::Statement *statement;

    /* The lexer returns these... */
    int integer;
    float real;
    char string[1024]; /* FIXME: We should make this bigger... */
}

/* Start tokens */
%token START_PROGRAM

/* Language structure */
%token TOKEN_EOS
%token TOKEN_BRACKETS_OPEN
%token TOKEN_BRACKETS_CLOSE
%token TOKEN_PARENTHESIS_OPEN
%token TOKEN_PARENTHESIS_CLOSE
%token TOKEN_BRACE_OPEN
%token TOKEN_BRACE_CLOSE
%token TOKEN_COMMA
%token TOKEN_DIRECT_ACCESS
%token TOKEN_MESSAGE_ACCESS

/* Operators */
%token TOKEN_LOGICAL_OR
%token TOKEN_LOGICAL_AND
%token TOKEN_OR
%token TOKEN_XOR
%token TOKEN_AND
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_LE
%token TOKEN_LT
%token TOKEN_GE
%token TOKEN_GT
%token TOKEN_ADD
%token TOKEN_SUBTRACT
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_MODULUS
%token TOKEN_LOGICAL_NOT
%token TOKEN_NOT
%token TOKEN_CAST
%token TOKEN_EQUALS

/* Basic Types */
%token<integer> TOKEN_INTEGER
%token<real> TOKEN_FLOAT
%token<string> TOKEN_STRING

/* Keywords */
%token TOKEN_INCLUDE
%token TOKEN_USE
%token TOKEN_GLOBAL
%token TOKEN_SHARED
%token TOKEN_CONST
%token TOKEN_VAR
%token TOKEN_REF
%token TOKEN_FUNCTION
%token TOKEN_RETURN
%token TOKEN_STATE

/* Built-In Types */
%token TOKEN_TYPE_INT
%token TOKEN_TYPE_FLOAT
%token TOKEN_TYPE_STRING

/* Identifiers */
%token TOKEN_NAME
%token TOKEN_ID

/* Start symbol */
%start start

%%

start               :   START_PROGRAM program
                    ;

program             :   o_program_includes o_program_uses o_program_cvrs o_program_functions
                        {
                            //yyerror("Test");
                        }
                    ;

o_program_includes  :   /* Empty */
                    |   program_includes
                    ;

program_includes    :   include_statement
                    |   program_includes include_statement
                    ;

include_statement   :   TOKEN_INCLUDE TOKEN_STRING TOKEN_EOS
                        {
                            FILE *input;

                            if((input = fopen($2, "r")))
                            {
                                yyscan_t newScanner;

                                yylex_init(&newScanner);
                                yyset_in(input, newScanner);

                                yyparse(newScanner);

                                yylex_destroy(newScanner);
                                fclose(input);
                            }
                            else
                            {
                                //emitError(locp->first_line, "Could not find include file ...");
                            }
                        }
                    ;

o_program_uses      :   /* Empty */
                    |   program_uses
                    ;

program_uses        :   use_statement
                    |   program_uses use_statement
                    ;

use_statement       :   TOKEN_USE TOKEN_NAME TOKEN_EOS
                    ;

o_program_cvrs      :   /* Empty */
                    |   program_cvrs
                    ;

program_cvrs        :   program_cvr                                                                 /* CVRs = constants + variables + references */
                    |   program_cvrs program_cvr
                        {
                            //$1->setSibling($2);
                        }
                    ;

program_cvr         :   constant_statement
                    |   program_variable
                    |   program_reference
                    ;

constant_statement  :   TOKEN_CONST name_type TOKEN_EQUALS expression TOKEN_EOS
                    ;

program_variable    :   TOKEN_GLOBAL variable_statement
                    |   TOKEN_SHARED variable_statement
                    |   variable_statement
                    ;

program_reference   :   TOKEN_GLOBAL reference_statement
                    |   TOKEN_SHARED reference_statement
                    |   reference_statement
                    ;

o_program_functions :   /* Empty */
                    |   program_functions
                    ;

program_functions   :   program_function
                    |   program_functions program_function
                        {
                            //$1->setSibling($2);
                        }
                    ;
program_function    :   function_prototype statement_block
                        {
                            //$1->setChild($2);
                        }
                    ;









function_prototype  :   TOKEN_FUNCTION id_type TOKEN_PARENTHESIS_OPEN argument_definitions TOKEN_PARENTHESIS_CLOSE
                        {
                            //$$ = new tree::FunctionPrototype($2, $4);
                        }
                    |   TOKEN_FUNCTION id_type TOKEN_PARENTHESIS_OPEN argument_definitions TOKEN_PARENTHESIS_CLOSE TOKEN_LT state_name TOKEN_GT
                        {
                            //$$ = new tree::FunctionPrototype($2, $4, $7);
                        }
                    ;

argument_definitions:   /* Empty */
                        {
                            //$$ = null;
                        }
                    |   id_type
                        {
                            //$$ = new tree::IdList();
                            //$$->push($1);
                        }
                    |   argument_definitions TOKEN_COMMA id_type
                        {
                            //$1->push($3);
                        }
                    ;



state_name          :   /* Empty (default state) */
                        {

                        }
                    |   TOKEN_NAME
                        {

                        }
                    ;



statements          :   statement
                    |   statements statement
                        {
                            //$1->setSibling($2);
                        }
                    ;

statement           :   variable_statement
                    |   reference_statement
                    |   assignment_statement
                    |   expression_statement    /* statements types here... */
                    |   return_statement
                    |   state_statement
                    |   statement_block

                    |   error_statement                                                             /* Special case!! */
                    ;

statement_block     :   TOKEN_BRACE_OPEN TOKEN_BRACE_CLOSE                                          /* Empty... */
                    |   TOKEN_BRACE_OPEN
                        {
                            //tree::Block *block = new tree::Block();

                            //tree::Block::getCurrentBlock()->addBlock(block);
                            //tree::Block::setCurrentBlock(block);
                        }
                        statements
                        {

                        }
                        TOKEN_BRACE_CLOSE
                        {
                            //tree::Block *block = tree::Block::getCurrentBlock();

                            //tree::Block::setCurrentBlock(block->getParent());
                        }
                    ;

variable_statement  :   TOKEN_VAR id_type TOKEN_EOS
                    |   TOKEN_VAR id_type TOKEN_EQUALS expression TOKEN_EOS
                    ;



reference_statement :   TOKEN_REF id_type TOKEN_EOS
                    |   TOKEN_REF id_type TOKEN_EQUALS expression TOKEN_EOS /* FIXME... */
                    ;

id_type             :   TOKEN_ID
                    |   type TOKEN_CAST TOKEN_ID
                    ;

name_type           :   TOKEN_NAME
                    |   type TOKEN_CAST TOKEN_NAME
                    ;

assignment_statement:   TOKEN_ID TOKEN_EQUALS expression TOKEN_EOS
                    ;

expression_statement:   expression TOKEN_EOS
                    ;

expression          :   l_or_expression
                    ;

l_or_expression     :   l_and_expression
                    |   l_or_expression TOKEN_LOGICAL_OR l_and_expression
                        {

                        }
                    ;

l_and_expression    :   or_expression
                    |   l_and_expression TOKEN_LOGICAL_AND or_expression
                        {

                        }
                    ;

or_expression       :   xor_expression
                    |   or_expression TOKEN_OR xor_expression
                        {

                        }
                    ;

xor_expression      :   and_expression
                    |   xor_expression TOKEN_XOR and_expression
                        {

                        }
                    ;

and_expression      :   eq_expression
                    |   and_expression TOKEN_AND eq_expression
                        {

                        }
                    ;

eq_expression       :   rel_expression
                    |   eq_expression TOKEN_EQ rel_expression
                        {

                        }
                    |   eq_expression TOKEN_NE rel_expression
                        {

                        }
                    ;

rel_expression      :   add_expression
                    |   rel_expression TOKEN_LT add_expression
                        {

                        }
                    |   rel_expression TOKEN_LE add_expression
                        {

                        }
                    |   rel_expression TOKEN_GT add_expression
                        {

                        }
                    |   rel_expression TOKEN_GE add_expression
                        {

                        }
                    ;

add_expression      :   mult_expression
                    |   add_expression TOKEN_ADD mult_expression
                        {

                        }
                    |   add_expression TOKEN_SUBTRACT mult_expression
                        {

                        }
                    ;

mult_expression     :   cast_expression
                    |   mult_expression TOKEN_MULTIPLY cast_expression
                        {

                        }
                    |   mult_expression TOKEN_DIVIDE cast_expression
                        {

                        }
                    |   mult_expression TOKEN_MODULUS cast_expression
                        {

                        }
                    ;

cast_expression     :   unary_expression
                    |   type TOKEN_CAST expression_atom
                        {

                        }
                    ;

unary_expression    :   access_expression
                    |   TOKEN_SUBTRACT access_expression
                        {

                        }
                    |   TOKEN_LOGICAL_NOT access_expression
                        {

                        }
                    |   TOKEN_NOT access_expression
                        {

                        }
                    ;

access_expression   :   postfix_expression ///////////////////////////////////////////////////////////////////////////////////////////////////
                    |   postfix_expression TOKEN_DIRECT_ACCESS postfix_expression
                        {

                        }
                    |   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression
                        {

                        }
                    ;

postfix_expression  :   expression_atom
                    |   array_expression
                    |   call_expression
                    ;

array_expression    :   postfix_expression TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                        {

                        }
                    ;

call_expression     :   postfix_expression TOKEN_PARENTHESIS_OPEN TOKEN_PARENTHESIS_CLOSE
                        {

                        }
                    |   postfix_expression TOKEN_PARENTHESIS_OPEN argument_expressions TOKEN_PARENTHESIS_CLOSE
                        {

                        }
                    ;

argument_expressions:   expression
                        {
                            
                        }
                    |   argument_expressions TOKEN_COMMA expression
                        {
                            
                        }
                    ;

expression_atom     :   TOKEN_NAME /* Constant */
                        {
                            //$$ = new tree::Identifier($1);
                        }
                    |   TOKEN_INTEGER
                        {
                            //$$ = new tree::LiteralInteger($1);
                        }
                    |   TOKEN_FLOAT
                        {
                            //$$ = new tree::LiteralFloat($1);
                        }
                    |   TOKEN_STRING
                        {
                            //$$ = new tree::LiteralString($1);
                        }
                    |   TOKEN_ID
                        {
                            //$$ = new tree::Identifier($1);
                        }
                    |   TOKEN_PARENTHESIS_OPEN expression TOKEN_PARENTHESIS_CLOSE
                        {
                            //$$ = $2;
                        }
                    ;

type                :   TOKEN_TYPE_INT
                        {
                            //$$ = new tree::Type(tree::TYPE_INT);
                        }
                    |   TOKEN_TYPE_FLOAT
                        {
                            //$$ = new tree::Type(tree::TYPE_FLOAT);
                        }
                    |   TOKEN_TYPE_STRING
                        {
                            //$$ = new tree::Type(tree::TYPE_STRING);
                        }
                    |   TOKEN_NAME
                        {
                            //$$ = new tree::Type(tree::TYPE_UDT, $1);
                        }
                    ;

return_statement    :   TOKEN_RETURN expression TOKEN_EOS
                        {
                            //$$ = new tree::ReturnStatement($2);
                        }
                    ;

state_statement     :   TOKEN_STATE state_name TOKEN_EOS /* FIXME, blank state name to reset to default is best?? */
                        {
                            //$$ = new tree::StateStatement($2);
                        }
                    ;

error_statement     :   error TOKEN_EOS
                    ;

%%

void yyerror(YYLTYPE *locp, void *scanner, const char *error)
{
    emitError(locp->first_line, error);
}

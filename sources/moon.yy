/*
 * moon.yy
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include "error.h"
    #include "tree.h"

    // Flex functions
    extern int yylex();
    extern void yyerror(const char *error);
%}

/* Better errors!! */
%error-verbose

/* Language structure */
%token TOKEN_EOS
%token TOKEN_EQUALS
%token TOKEN_PARENTHESIS_OPEN
%token TOKEN_PARENTHESIS_CLOSE
%token TOKEN_BRACE_OPEN
%token TOKEN_BRACE_CLOSE
%token TOKEN_CAST

/* Basic Types */
%token TOKEN_INTEGER
%token TOKEN_FLOAT

/* Strings */
%token TOKEN_STRING

/* Keywords */
%token TOKEN_INCLUDE
%token TOKEN_USE
%token TOKEN_GLOBAL
%token TOKEN_SHARED
%token TOKEN_VAR
%token TOKEN_REF

/* Identifiers */
%token TOKEN_ID_INTEGER
%token TOKEN_ID_FLOAT
%token TOKEN_ID_CLASS
%token TOKEN_ID_UNKNOWN

/* Start symbol */
%start program

%%

program             :   program_includes program_uses program_vrs program_functions
                        {
                            //yyerror("Test");
                        }
                    ;

program_includes    :   /* Empty */
                    |   program_includes include_statement
                    |   include_statement
                    ;

include_statement   :   TOKEN_INCLUDE TOKEN_STRING TOKEN_EOS
                    ;

program_uses        :   /* Empty */
                    |   program_uses use_statement
                    |   use_statement
                    ;

use_statement       :   TOKEN_USE TOKEN_ID_CLASS TOKEN_EOS
                    ;

program_vrs         :   /* Empty */                                                                 /* VRs = variables + references */
                    |   program_vrs program_vr
                    |   program_vr
                    ;

program_vr          :   program_variable
                    |   program_reference
                    ;

program_variable    :   TOKEN_GLOBAL variable_statement
                    |   TOKEN_SHARED variable_statement
                    |   variable_statement
                    ;

program_reference   :   TOKEN_GLOBAL reference_statement
                    |   TOKEN_SHARED reference_statement
                    |   reference_statement
                    ;

program_functions   :   /* Empty */
                    |   function_prototype statement_block
                    ;










function_prototype  :   TOKEN_ID_UNKNOWN TOKEN_PARENTHESIS_OPEN TOKEN_PARENTHESIS_CLOSE
                    ;





statements          :   statement
                    |   statements statement
                    ;

statement           :   variable_statement
                    |   reference_statement
                    |   expression_statement    /* statements types here... */
                    |   statement_block
                    ;

statement_block     :   TOKEN_BRACE_OPEN TOKEN_BRACE_CLOSE                                          /* Empty... */
                    |   block_start statements block_end
                    ;

block_start         :   TOKEN_BRACE_OPEN
                        {
                            tree::Block *block = new tree::Block();

                            tree::Block::getCurrentBlock()->addBlock(block);
                            tree::Block::setCurrentBlock(block);
                        }
                    ;

block_end           :   TOKEN_BRACE_CLOSE
                        {
                            tree::Block *block = tree::Block::getCurrentBlock();

                            tree::Block::setCurrentBlock(block->getParent());
                        }
                    ;

variable_statement  :   TOKEN_VAR TOKEN_ID_UNKNOWN TOKEN_EOS
                    |   TOKEN_VAR TOKEN_ID_UNKNOWN TOKEN_EQUALS expression TOKEN_EOS
                    ;

reference_statement :   TOKEN_REF TOKEN_ID_UNKNOWN TOKEN_EOS
                    |   TOKEN_REF TOKEN_ID_UNKNOWN TOKEN_EQUALS expression TOKEN_EOS /* FIXME... */
                    ;

expression_statement:   expression TOKEN_EOS
                    ;

expression          :   TOKEN_ID_UNKNOWN /* FIXME */
                    |   numeric_expression
                    ;

numeric_expression  :   integer_expression
                    |   float_expression
                    ;

integer_expression  :   TOKEN_INTEGER
                    |   TOKEN_ID_INTEGER
                    ;

float_expression    :   TOKEN_FLOAT
                    |   TOKEN_ID_FLOAT
                    ;

%%

/*
 * moon.yy
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include "error.h"

    // Flex functions
    extern int yylex();
    extern void yyerror(const char *error);
%}

/* Better errors!! */
%error-verbose

/* Language structure */
%token TOKEN_EOS
%token TOKEN_PARENTHESIS_OPEN
%token TOKEN_PARENTHESIS_CLOSE
%token TOKEN_BRACE_OPEN
%token TOKEN_BRACE_CLOSE

/* Basic Types */
%token TOKEN_INTEGER
%token TOKEN_FLOAT

/* Strings */
%token TOKEN_STRING

/* Keywords */
%token TOKEN_GLOBAL
%token TOKEN_SHARED
%token TOKEN_VAR
%token TOKEN_REF

/* Identifiers */
%token TOKEN_IDENTIFIER

/* Start symbol */
%start program

%%

program             :   program_includes program_vrs program_functions
                        {
                            //yyerror("Test");
                        }
                    ;

program_includes    :   /* Empty */
                    |   
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










function_prototype  :   TOKEN_IDENTIFIER TOKEN_PARENTHESIS_OPEN TOKEN_PARENTHESIS_CLOSE
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
                    |   TOKEN_BRACE_OPEN statements TOKEN_BRACE_CLOSE
                    ;

variable_statement  :   TOKEN_VAR TOKEN_IDENTIFIER TOKEN_EOS
                    ;

reference_statement :   TOKEN_REF TOKEN_IDENTIFIER TOKEN_EOS
                    ;

expression_statement:   expression TOKEN_EOS
                    ;

expression          :   TOKEN_IDENTIFIER
                    ;

%%

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
%}

/* Better errors!! */
%error-verbose

/* Language structure */
%token TOKEN_EOS

/* Basic Types */
%token TOKEN_INTEGER
%token TOKEN_FLOAT

/* Strings */
%token TOKEN_STRING

/* Keywords */


/* Identifiers */
%token TOKEN_IDENTIFIER

/* Start symbol */
%start program

%%

program				:	program_includes	program_variables	program_functions
						{
							//yyerror("Test");
						}
					;

program_includes	:	/*  */
					|	
					;

program_variables	:	/*  */
					|	"global" variable_definition
					|	variable_definition
					;

program_functions	:	/*  */
					|	
					;





variable_definition	:
					;	






statements			:	statement
					|	statements	statement
					;

statement			:	TOKEN_IDENTIFIER
					;

%%

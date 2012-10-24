/*
 * moon.ll
 * Flex parser for moon
 *
 * Unfortunately, nice-looking Flex is really REALLY hard to write. :(
 * It's pretty ugly, but please try and match your style to what's already in this file...
 */

%{
	#include "error.h"
	#include "parser.h"
%}

%option noyywrap
%option yylineno

%x LINE_COMMENT
%x BLOCK_COMMENT
%x STRING

%%
	/* Language structure */

[ \t\n]						;													/* Skip whitespace... */
";"							return TOKEN_EOS;

	/* Basic Types */
[0-9]+						return TOKEN_INTEGER;
[0-9]*\.[0-9]+				return TOKEN_FLOAT;

	/* Strings */
"\""						BEGIN STRING;
<STRING>"\""				BEGIN 0; return TOKEN_STRING;

	/* Keywords */
"global"					return TOKEN_GLOBAL;
"var"						return TOKEN_VAR;

	/* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]* 		return TOKEN_IDENTIFIER;

	/* Comments */
"//"						BEGIN LINE_COMMENT;									/* One line comments... */
<LINE_COMMENT>.				;
<LINE_COMMENT>"\n"			BEGIN 0;
<LINE_COMMENT><<EOF>>		BEGIN 0;
"/*"						BEGIN BLOCK_COMMENT;								/* Block comments... */
<BLOCK_COMMENT>.			;
<BLOCK_COMMENT>"*/"			BEGIN 0;

	/* Anything else...*/
.							return yytext[0];

%%

void yyerror(const char *error)
{
	emitError(yylineno, error);
}

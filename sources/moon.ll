/*
 * moon.ll
 * Flex parser for moon
 *
 * Unfortunately, nice-looking Flex is really REALLY hard to write. :(
 * It's pretty ugly, but please try and match your style to what's already in this file...
 */

%{
	#include "parser.h"
	#include <iostream> // DEBUG
%}

%option noyywrap

%x COMMENT
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

	/* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]* 		return TOKEN_IDENTIFIER;

	/* Comments */
"//".*\n					;													/* One line comments... */
"/*"						BEGIN COMMENT;										/* Block comments... */
<COMMENT>.					;
<COMMENT>"*/"				BEGIN 0;

	/* Anything else...*/
.							return yytext[0];

%%

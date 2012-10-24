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

/* States */
%x STRING
%x LINE_COMMENT
%x BLOCK_COMMENT

%%
	/* Language structure */

[ \t\n]                         ;                                                                   /* Skip whitespace... */
";"                             return TOKEN_EOS;
"("                             return TOKEN_PARENTHESIS_OPEN;
")"                             return TOKEN_PARENTHESIS_CLOSE;
"{"                             return TOKEN_BRACE_OPEN;
"}"                             return TOKEN_BRACE_CLOSE;

	/* Basic Types */
[0-9]+                          return TOKEN_INTEGER;
[0-9]*\.[0-9]+                  return TOKEN_FLOAT;

	/* Strings */
"\""                            BEGIN STRING;
<STRING>"\""                    BEGIN 0; return TOKEN_STRING;

	/* Keywords */
"include"                       return TOKEN_INCLUDE;
"use"                           return TOKEN_USE;
"global"                        return TOKEN_GLOBAL;
"shared"                        return TOKEN_SHARED;
"var"                           return TOKEN_VAR;
"ref"                           return TOKEN_REF;

	/* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*          return TOKEN_IDENTIFIER;

	/* Line Comments */
"//"                            BEGIN LINE_COMMENT;                                                 /* One line comments... */
<LINE_COMMENT>.                 ;
<LINE_COMMENT>"\n"              BEGIN 0;
<LINE_COMMENT><<EOF>>           BEGIN 0;

	/* Block Comments */
"/*"                            BEGIN BLOCK_COMMENT;                                                /* Block comments... */
<BLOCK_COMMENT>.                ;
<BLOCK_COMMENT>"*/"             BEGIN 0;

	/* Anything else...*/
.                               return yytext[0];

%%

void yyerror(const char *error)
{
	emitError(yylineno, error);
}

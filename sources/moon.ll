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
    #include "tree.h"

    static unsigned int sStringLength;
%}

%option noyywrap
%option yylineno

/* States */
%x STRING_LITERAL
%x LINE_COMMENT
%x BLOCK_COMMENT

%%
    /* Language structure */

[ \t\n]                         ;                                                                   /* Skip whitespace... */
";"                             return TOKEN_EOS;
"="                             return TOKEN_EQUALS;
"("                             return TOKEN_PARENTHESIS_OPEN;
")"                             return TOKEN_PARENTHESIS_CLOSE;
"{"                             return TOKEN_BRACE_OPEN;
"}"                             return TOKEN_BRACE_CLOSE;
":"                             return TOKEN_CAST;
"."                             return TOKEN_DIRECT_ACCESS;
"->"                            return TOKEN_MESSAGE_ACCESS;

    /* Basic Types */
[0-9]+                          {
                                    yylval.integer = atoi(yytext);
                                    return TOKEN_INTEGER;
                                }
[0-9]*\.[0-9]+                  {
                                    yylval.real = (float)atof(yytext);
                                    return TOKEN_FLOAT;
                                }

    /* Strings */
"\""                            {
                                    BEGIN STRING_LITERAL;
                                    sStringLength = 0;
                                }
<STRING_LITERAL>"\""            {
                                    BEGIN INITIAL;
                                    yylval.string[sStringLength] = 0;
                                    return TOKEN_STRING;
                                }
<STRING_LITERAL>.               yylval.string[sStringLength++] = *yytext;

    /* Keywords */
"include"                       return TOKEN_INCLUDE;
"use"                           return TOKEN_USE;
"global"                        return TOKEN_GLOBAL;
"shared"                        return TOKEN_SHARED;
"const"                         return TOKEN_CONST;
"var"                           return TOKEN_VAR;
"ref"                           return TOKEN_REF;
"function"                      return TOKEN_FUNCTION;

    /* Built-In Types */
"int"                           return TOKEN_TYPE_INT;
"float"                         return TOKEN_TYPE_FLOAT;

    /* Identifiers */
[A-Z][A-Z0-9]*                  return TOKEN_ID_CONSTANT; /*FIXME*/
[A-Z][a-zA-Z0-9]*               return TOKEN_ID_CLASS;    /*FIXME*/
[a-z_][a-zA-Z0-9_]*             return TOKEN_ID;

    /* Line Comments */
"//"                            BEGIN LINE_COMMENT;                                                 /* One line comments... */
<LINE_COMMENT>.                 ;
<LINE_COMMENT>"\n"              BEGIN INITIAL;
<LINE_COMMENT><<EOF>>           BEGIN INITIAL;

    /* Block Comments */
"/*"                            BEGIN BLOCK_COMMENT;                                                /* Block comments... */
<BLOCK_COMMENT>.                ;
<BLOCK_COMMENT>"*/"             BEGIN INITIAL;

    /* Anything else...*/
.                               return yytext[0];

%%

void yyerror(const char *error)
{
    emitError(yylineno, error);
}

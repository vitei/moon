/*
 * moon.ll
 * Flex parser for moon
 *
 * Unfortunately, nice-looking Flex is really REALLY hard to write. :(
 * It's pretty ugly, but please try and match your style to what's already in this file...
 */

%{
    #include "error.h"
    #include "tree.h"

    /* Generated headers */
    #include "parser.h"

    /* This should let us have line numbers... */
    #define YY_USER_ACTION yylloc->first_line = yylineno;

    static unsigned int sStringLength;
%}

/* Re-entrant */
%option reentrant

%option noyywrap
%option bison-bridge

/* Program locations */
%option yylineno
%option bison-locations

/* States */
%x STRING_LITERAL
%x LINE_COMMENT
%x BLOCK_COMMENT

%%

%{
    static bool startSymbolIssued = false;

    if(!startSymbolIssued)
    {
        startSymbolIssued = true;
        return START_PROGRAM;
    }
%}

    /* Language structure */
[ \t\n]                         ;                                                                   /* Skip whitespace... */
";"                             return TOKEN_EOS;
"["                             return TOKEN_BRACKETS_OPEN;
"]"                             return TOKEN_BRACKETS_CLOSE;
"("                             return TOKEN_PARENTHESIS_OPEN;
")"                             return TOKEN_PARENTHESIS_CLOSE;
"{"                             return TOKEN_BRACE_OPEN;
"}"                             return TOKEN_BRACE_CLOSE;
","                             return TOKEN_COMMA;
"."                             return TOKEN_DIRECT_ACCESS;
"->"                            return TOKEN_MESSAGE_ACCESS;

    /* Operators */
"||"                            return TOKEN_LOGICAL_OR;
"&&"                            return TOKEN_LOGICAL_AND;
"|"                             return TOKEN_OR;
"^"                             return TOKEN_XOR;
"&"                             return TOKEN_AND;
"=="                            return TOKEN_EQ;
"!="                            return TOKEN_NE;
"<="                            return TOKEN_LE;
"<"                             return TOKEN_LT;
">="                            return TOKEN_GE;
">"                             return TOKEN_GT;
"+"                             return TOKEN_ADD;
"-"                             return TOKEN_SUBTRACT;
"*"                             return TOKEN_MULTIPLY;
"/"                             return TOKEN_DIVIDE;
"%"                             return TOKEN_MODULUS;
"!"                             return TOKEN_LOGICAL_NOT;
"~"                             return TOKEN_NOT;
":"                             return TOKEN_CAST;
"="                             return TOKEN_EQUALS;

    /* Basic Types */
[0-9]+                          {
                                    yylval->integer = atoi(yytext);
                                    return TOKEN_INTEGER;
                                }
[0-9]*\.[0-9]+                  {
                                    yylval->real = (float)atof(yytext);
                                    return TOKEN_FLOAT;
                                }

    /* Strings */
"\""                            {
                                    BEGIN STRING_LITERAL;
                                    sStringLength = 0;
                                }
<STRING_LITERAL>"\""            {
                                    BEGIN INITIAL;
                                    yylval->string[sStringLength] = 0;
                                    return TOKEN_STRING;
                                }
<STRING_LITERAL>.               yylval->string[sStringLength++] = *yytext;

    /* Keywords */
"include"                       return TOKEN_INCLUDE;
"use"                           return TOKEN_USE;
"global"                        return TOKEN_GLOBAL;
"shared"                        return TOKEN_SHARED;
"const"                         return TOKEN_CONST;
"var"                           return TOKEN_VAR;
"ref"                           return TOKEN_REF;
"function"                      return TOKEN_FUNCTION;
"return"                        return TOKEN_RETURN;
"state"                         return TOKEN_STATE;

    /* Built-In Types */
"int"                           return TOKEN_TYPE_INT;
"float"                         return TOKEN_TYPE_FLOAT;
"string"                        return TOKEN_TYPE_STRING;

    /* Identifiers */
[A-Z][a-zA-Z0-9_]*              return TOKEN_NAME;
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

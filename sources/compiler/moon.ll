/*
 * moon.ll
 * Flex parser for moon
 *
 * Unfortunately, nice-looking Flex is really REALLY hard to write. :(
 * It's pretty ugly, but please try and match your style to what's already in this file...
 */

%{
    #include "compiler/lexer.h"
    #include "compiler/parser.h"
    #include "compiler/tree.h"

    /* Generated headers */
    #include "generated/parser.h"

    #define YY_USER_ACTION yylloc->advanceCharacter(yyleng);

    static unsigned int sStringLength;
%}

/* Extra data */
%option extra-type="lexer::Data *"

/* Re-entrant */
%option reentrant

%option never-interactive
%option noyywrap
%option bison-bridge

/* Program locations */
%option bison-locations

/* States */
%x PRIMED
%x STRING_LITERAL
%x LINE_COMMENT
%x BLOCK_COMMENT

%%

%{
    if(!yyextra->startSymbolIssued)
    {
        yyextra->startSymbolIssued = true;

        switch(yyextra->type)
        {
        case lexer::Data::TYPE_USE:
            return START_USE;

        case lexer::Data::TYPE_INCLUDE:
            return START_INCLUDE;
        }
    }
%}

    /* Skip whitespace... */
[ \t]+                  yylloc->advance();
[\n\r]+                 {
                            yylloc->advanceLine(yyleng);
                            yylloc->advance();
                        }

    /* Comments */
"#"                     BEGIN LINE_COMMENT;
"-#-"                   BEGIN BLOCK_COMMENT;

    /* Anything else primes the lexer */
.                       {
                            BEGIN PRIMED;
                            yyless(0);
                        }

<PRIMED>{
    /* Skip whitespace... */
    [ \t]+              yylloc->advance();

    /* Language structure */
    [\n\r]+             {
                            yylloc->advanceLine(yyleng);
                            yylloc->advance();

                            BEGIN INITIAL;
                            return TOKEN_EOS;
                        }
    <<EOF>>             {
                            BEGIN INITIAL;
                            return TOKEN_EOS;
                        }
    ";"                 return TOKEN_SEMICOLON;
    "["                 return TOKEN_BRACKETS_OPEN;
    "]"                 return TOKEN_BRACKETS_CLOSE;
    "("                 return TOKEN_PARENTHESIS_OPEN;
    ")"                 return TOKEN_PARENTHESIS_CLOSE;
    "{"                 return TOKEN_BRACE_OPEN;
    "}"                 return TOKEN_BRACE_CLOSE;
    ","                 return TOKEN_COMMA;
    "."                 return TOKEN_DIRECT_ACCESS;
    "->"                return TOKEN_MESSAGE_ACCESS;

    /* Operators */
    "||"                return TOKEN_LOGICAL_OR;
    "&&"                return TOKEN_LOGICAL_AND;
    "|"                 return TOKEN_OR;
    "^"                 return TOKEN_XOR;
    "&"                 return TOKEN_AND;
    "=="                return TOKEN_EQ;
    "!="                return TOKEN_NE;
    "<="                return TOKEN_LE;
    "<"                 return TOKEN_LT;
    ">="                return TOKEN_GE;
    ">"                 return TOKEN_GT;
    "+"                 return TOKEN_ADD;
    "-"                 return TOKEN_SUBTRACT;
    "*"                 return TOKEN_MULTIPLY;
    "/"                 return TOKEN_DIVIDE;
    "%"                 return TOKEN_MODULUS;
    "!"                 return TOKEN_LOGICAL_NOT;
    "~"                 return TOKEN_NOT;
    "|="                return TOKEN_OR_EQUALS;
    "^="                return TOKEN_XOR_EQUALS;
    "&="                return TOKEN_AND_EQUALS;
    "+="                return TOKEN_ADD_EQUALS;
    "-="                return TOKEN_SUBTRACT_EQUALS;
    "*="                return TOKEN_MULTIPLY_EQUALS;
    "/="                return TOKEN_DIVIDE_EQUALS;
    "%="                return TOKEN_MODULUS_EQUALS;
    ":"                 return TOKEN_CAST;
    "="                 return TOKEN_EQUALS;

    /* Basic Types */
    [0-9]+              {
                            yylval->integer = atoi(yytext);
                            return TOKEN_INTEGER;
                        }
    0x[0-9A-Fa-f]+      {
                            sscanf(yytext, "0x%x", &yylval->integer);
                            return TOKEN_INTEGER;
                        }
    [0-9]+\.[0-9]+      {
                            yylval->real = (float)atof(yytext);
                            return TOKEN_FLOAT;
                        }

    /* Strings */
    "\""                {
                            BEGIN STRING_LITERAL;
                            sStringLength = 0;
                        }

    /* Keywords */
    "null"              return TOKEN_NULL;
    "true"              return TOKEN_TRUE;
    "false"             return TOKEN_FALSE;
    "end"               return TOKEN_END;
    "include"           return TOKEN_INCLUDE;
    "use"               return TOKEN_USE;
    "import"            return TOKEN_IMPORT;
    "global"            return TOKEN_GLOBAL;
    "shared"            return TOKEN_SHARED;
    "const"             return TOKEN_CONST;
    "var"               return TOKEN_VAR;
    "ref"               return TOKEN_REF;
    "function"          return TOKEN_FUNCTION;
    "return"            return TOKEN_RETURN;
    "state"             return TOKEN_STATE;
    "reset"             return TOKEN_RESET;

    /* Built-In Types */
    "bool"              return TOKEN_TYPE_BOOL;
    "int"               return TOKEN_TYPE_INT;
    "float"             return TOKEN_TYPE_FLOAT;
    "string"            return TOKEN_TYPE_STRING;

    /* Identifiers */
    [A-Z][a-zA-Z0-9_]*  {
                            strcpy(yylval->string, yytext);
                            return TOKEN_NAME;
                        }
    [a-z_][a-zA-Z0-9_]* {
                            strcpy(yylval->string, yytext);
                            return TOKEN_ID;
                        }

    /* Comments */
    "#"                 {
                            BEGIN LINE_COMMENT;
                            return TOKEN_EOS;
                        }

    /* Anything else...*/
    .                   return yytext[0];
}

<STRING_LITERAL>{
    /* Strings */
    "\""                {
                            BEGIN PRIMED;
                            yylval->string[sStringLength] = 0;
                            return TOKEN_STRING;
                        }
    .                   yylval->string[sStringLength++] = *yytext;
}

<LINE_COMMENT>{
    /* Line Comments */
    [\n\r]+             {
                            yylloc->advanceLine(yyleng);
                            BEGIN INITIAL;
                        }
    <<EOF>>             {
                            yylloc->advanceLine(yyleng);
                            BEGIN INITIAL;
                        }
    .                   ;
}

<BLOCK_COMMENT>{
    /* Block Comments */
    "-#-"[ \t]*[\n\r]   {
                            yylloc->advanceLine(1);
                            BEGIN INITIAL;
                        }
    [\n\r]+             yylloc->advanceLine(yyleng);
    .                   ;
}

%%

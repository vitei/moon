/*
 * moon.l
 * Flex parser for moon
 *
 * Unfortunately, nice-looking Flex is really REALLY hard to write. :(
 * It's pretty ugly, but please try and match your style to what's already in this file...
 */

%{
    #include "common.h"

    #include <string>

    #include "compiler/error.h"
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
%x CODE
%x PRIMED
%x STRING_LITERAL
%x STRING_LITERAL_ESCAPE
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
            BEGIN CODE;
        case lexer::Data::TYPE_LITERATE_USE:
            return START_USE;

        case lexer::Data::TYPE_INCLUDE:
            BEGIN CODE;
            return START_INCLUDE;
        }
    }
%}

    /* Literate moon */
[\n\r]+                     {
                                yylloc->advanceLine(yyleng);
                                yylloc->advance();
                            }
^">"                        BEGIN CODE;
.                           ;

<CODE>{
    /* Skip whitespace... */
    [ \t]+                  yylloc->advance();
    [\n\r]+                 {
                                yylloc->advanceLine(yyleng);
                                yylloc->advance();

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                            }

    /* Comments */
    "#"                     BEGIN LINE_COMMENT;
    "-#-"                   BEGIN BLOCK_COMMENT;

    /* Anything else primes the lexer */
    .                       {
                                BEGIN PRIMED;
                                yyless(0);
                            }
}

<PRIMED>{
   /* Join lines */
    [ \t]+"..."[\n\r]       {
                                // Use ellipsis to break up big lines
                                yylloc->advanceLine(1);
                                yylloc->advance();
                            }

    /* Skip whitespace... */
    [ \t]+                  yylloc->advance();

    /* Language structure */
    [\n\r]+                 {
                                yylloc->advanceLine(yyleng);
                                yylloc->advance();

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }
    <<EOF>>                 {
                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }
    ";"                     return TOKEN_SEMICOLON;
    "["                     return TOKEN_BRACKETS_OPEN;
    "]"                     return TOKEN_BRACKETS_CLOSE;
    "("                     return TOKEN_PARENTHESIS_OPEN;
    ")"                     return TOKEN_PARENTHESIS_CLOSE;
    "{"                     return TOKEN_BRACE_OPEN;
    "}"                     return TOKEN_BRACE_CLOSE;
    ","                     return TOKEN_COMMA;
    "."                     return TOKEN_DIRECT_ACCESS;
    "->"                    return TOKEN_MESSAGE_ACCESS;

    /* Operators */
    "||"                    return TOKEN_LOGICAL_OR;
    "&&"                    return TOKEN_LOGICAL_AND;
    "|"                     return TOKEN_OR;
    "^"                     return TOKEN_XOR;
    "&"                     return TOKEN_AND;
    "=="                    return TOKEN_EQ;
    "!="                    return TOKEN_NE;
    "<="                    return TOKEN_LE;
    "<"                     return TOKEN_LT;
    ">="                    return TOKEN_GE;
    ">"                     return TOKEN_GT;
    "+"                     return TOKEN_ADD;
    "-"                     return TOKEN_SUBTRACT;
    "*"                     return TOKEN_MULTIPLY;
    "/"                     return TOKEN_DIVIDE;
    "%"                     return TOKEN_MODULUS;
    "!"                     return TOKEN_LOGICAL_NOT;
    "~"                     return TOKEN_NOT;
    "|="                    return TOKEN_OR_EQUALS;
    "^="                    return TOKEN_XOR_EQUALS;
    "&="                    return TOKEN_AND_EQUALS;
    "+="                    return TOKEN_ADD_EQUALS;
    "-="                    return TOKEN_SUBTRACT_EQUALS;
    "*="                    return TOKEN_MULTIPLY_EQUALS;
    "/="                    return TOKEN_DIVIDE_EQUALS;
    "%="                    return TOKEN_MODULUS_EQUALS;
    ":"                     return TOKEN_CAST;
    "="                     return TOKEN_EQUALS;

    /* Basic Types */
    [0-9]+\.[0-9]+          {
                                yylval->real = (float)atof(yytext);
                                return TOKEN_FLOAT;
                            }
    [0-1]+":b"              {
                                char *castPos = strchr(yytext, ':');
                                *castPos = 0;
                                yylval->integer = strtol(yytext, NULL, 2);

                                return TOKEN_INTEGER;
                            }
    [0-8]+"o:"              {
                                char *castPos = strchr(yytext, ':');
                                *castPos = 0;
                                yylval->integer = strtol(yytext, NULL, 8);

                                return TOKEN_INTEGER;
                            }
    [0-9A-Fa-f]+"h:"        {
                                char *castPos = strchr(yytext, ':');
                                *castPos = 0;
                                yylval->integer = strtol(yytext, NULL, 16);

                                return TOKEN_INTEGER;
                            }
    [0-9A-Za-z]+":"[0-9]+   {
                                char *baseString = strchr(yytext, ':');

                                // Get rid of the ":"
                                *baseString++ = 0;

                                int base = atoi(baseString);

                                if(base >= 2 && base <= 36)
                                {
                                    yylval->integer = 0;

                                    for(char *i = yytext; *i; i++)
                                    {
                                        if(*i != '0')
                                        {
                                            yylval->integer = strtol(yytext, NULL, base);

                                            if(!yylval->integer)
                                            {
                                                std::string error = std::string("Invalid number \"") + yytext + "\" for base " + baseString;
                                                error::enqueue(*yylloc, error);
                                            }

                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    std::string error("Invalid base \"");
                                    error += yytext;
                                    error += "\"";

                                    error::enqueue(*yylloc, error);

                                    yylval->integer = 0;
                                }

                                return TOKEN_INTEGER;
                            }
    [0-9]+                  {
                                yylval->integer = atoi(yytext);
                                return TOKEN_INTEGER;
                            }

    /* Strings */
    "\""                    {
                                BEGIN STRING_LITERAL;
                                sStringLength = 0;
                            }

    /* Keywords */
    "true"                  return TOKEN_TRUE;
    "false"                 return TOKEN_FALSE;
    "end"                   return TOKEN_END;
    "include"               return TOKEN_INCLUDE;
    "use"                   return TOKEN_USE;
    "import"                return TOKEN_IMPORT;
    "global"                return TOKEN_GLOBAL;
    "shared"                return TOKEN_SHARED;
    "const"                 return TOKEN_CONST;
    "def"                   return TOKEN_DEF;
    "if"                    return TOKEN_IF;
    "else"                  return TOKEN_ELSE;
    "while"                 return TOKEN_WHILE;
    "return"                return TOKEN_RETURN;
    /*"state"                 return TOKEN_STATE;*/
    /*"reset"                 return TOKEN_RESET;*/

    /* Built-In Types */
    "bool"                  return TOKEN_TYPE_BOOL;
    "int"                   return TOKEN_TYPE_INT;
    "float"                 return TOKEN_TYPE_FLOAT;
    "string"                return TOKEN_TYPE_STRING;

    /* Identifiers */
    "c."[A-Z][a-zA-Z0-9_]*  { // FIXME, this is a very hacky way to let us use C functions with upper-case names.
                                strcpy(yylval->string, &yytext[2]);
                                return TOKEN_ID;
                            }
    [A-Z][a-zA-Z0-9_]*      {
                                strcpy(yylval->string, yytext);
                                return TOKEN_NAME;
                            }
    [a-z_][a-zA-Z0-9_]*     {
                                strcpy(yylval->string, yytext);
                                return TOKEN_ID;
                            }

    /* Comments */
    "#"                     {
                                BEGIN LINE_COMMENT;
                                return TOKEN_EOS;
                            }

    /* Anything else...*/
    .                       return yytext[0];
}

<STRING_LITERAL>{
    [\n\r]+                 {
                                error::enqueue(*yylloc, "Premature end of string");

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }
    <<EOF>>                 {
                                error::enqueue(*yylloc, "Premature end of string");

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }

    /* Strings */
    "\""                    {
                                BEGIN PRIMED;
                                yylval->string[sStringLength] = 0;
                                return TOKEN_STRING;
                            }
    "\\"                    BEGIN STRING_LITERAL_ESCAPE;
    .                       yylval->string[sStringLength++] = *yytext;
}

<STRING_LITERAL_ESCAPE>{
    [\n\r]+                 {
                                error::enqueue(*yylloc, "Premature end of string");

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }
    <<EOF>>                 {
                                error::enqueue(*yylloc, "Premature end of string");

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }

                                return TOKEN_EOS;
                            }

    /* Strings */
    "\\"                    {
                                yylval->string[sStringLength++] = '\\';
                                BEGIN STRING_LITERAL;
                            }
    "\'"                    {
                                yylval->string[sStringLength++] = '\'';
                                BEGIN STRING_LITERAL;
                            }
    "\""                    {
                                yylval->string[sStringLength++] = '\"';
                                BEGIN STRING_LITERAL;
                            }
    "a"                     {
                                yylval->string[sStringLength++] = '\a';
                                BEGIN STRING_LITERAL;
                            }
    "b"                     {
                                yylval->string[sStringLength++] = '\b';
                                BEGIN STRING_LITERAL;
                            }
    "f"                     {
                                yylval->string[sStringLength++] = '\f';
                                BEGIN STRING_LITERAL;
                            }
    "n"                     {
                                yylval->string[sStringLength++] = '\n';
                                BEGIN STRING_LITERAL;
                            }
    "r"                     {
                                yylval->string[sStringLength++] = '\r';
                                BEGIN STRING_LITERAL;
                            }
    "t"                     {
                                yylval->string[sStringLength++] = '\t';
                                BEGIN STRING_LITERAL;
                            }
    .                       {
                                std::string error("Invalid escape character \"");
                                error += *yytext;
                                error += "\"";

                                error::enqueue(*yylloc, error);
                                BEGIN STRING_LITERAL;
                            }
}

<LINE_COMMENT>{
    /* Line Comments */
    [\n\r]+                 {
                                yylloc->advanceLine(yyleng);

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }
                            }
    <<EOF>>                 {
                                yylloc->advanceLine(yyleng);

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }
                            }
    .                       ;
}

<BLOCK_COMMENT>{
    /* Block Comments */
    "-#-"[ \t]*[\n\r]       {
                                yylloc->advanceLine(1);

                                if(yyextra->type == lexer::Data::TYPE_LITERATE_USE)
                                {
                                    BEGIN INITIAL;
                                }
                                else
                                {
                                    BEGIN CODE;
                                }
                            }
    [\n\r]+                 yylloc->advanceLine(yyleng);
    .                       ;
}

%%

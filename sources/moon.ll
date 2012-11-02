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
%}

%option noyywrap
%option yylineno

/* States */
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

    /* Basic Types */
[0-9]+                          yylval.integer = atoi(yytext); return TOKEN_INTEGER;
[0-9]*\.[0-9]+                  yylval.real = (float)atof(yytext); return TOKEN_FLOAT;

    /* Strings */
\"[^\n"]+\"                     strcpy(yylval.string, yytext); return TOKEN_STRING;

    /* Keywords */
"include"                       return TOKEN_INCLUDE;
"use"                           return TOKEN_USE;
"global"                        return TOKEN_GLOBAL;
"shared"                        return TOKEN_SHARED;
"var"                           return TOKEN_VAR;
"ref"                           return TOKEN_REF;

    /* Identifiers */
[A-Z][a-zA-Z0-9]*               return TOKEN_ID_CLASS;
[a-z_][a-zA-Z0-9_]*             {
                                    tree::Block *block = tree::Block::getCurrentBlock();
                                    std::string name(yytext);
                                    tree::Identifier *identifier = block->getIdentifier(name);

                                    if(identifier)
                                    {
                                        switch(identifier->getIdentifierType())
                                        {
                                            case tree::Identifier::TYPE_VARIABLE:
                                                {
                                                    tree::Variable *variable = static_cast<tree::Variable *>(identifier);

                                                    switch(variable->getVariableType())
                                                    {
                                                        case tree::Variable::TYPE_INTEGER:
                                                            return TOKEN_ID_INTEGER;

                                                        case tree::Variable::TYPE_FLOAT:
                                                            return TOKEN_ID_FLOAT;
                                                    }

                                                    break;
                                                }

                                            //case tree::Identifier::TYPE_REFERENCE:
                                            //    {
                                            //        break;
                                            //    }
                                        }
                                    }
                                    else
                                    {
                                        return TOKEN_ID_UNKNOWN;
                                    }
                                }

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

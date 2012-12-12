/*
 * moon.yy
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include <cstdio>
    #include <libgen.h>
    #include "compiler/error.h"
    #include "compiler/lexer.h"
    #include "compiler/loader.h"
    #include "compiler/parser.h"
    #include "tree.h"

    /* Generated headers */
    #include "generated_parser.h"
    #include "generated_lexer.h"

    #define scanner data->lexer

    void yyerror(YYLTYPE *locp, void *lexer, const char *error);
%}

/* Re-entrant */
%pure-parser
%parse-param {parser::Data *data}
%lex-param {void *scanner}

/* Program locations */
%locations

/* Better errors!! */
%error-verbose

/*  */
%union {
    tree::Use *use;

    tree::Statement *statement;
    tree::StatementList *statementList;

    tree::Expression *expression;
    tree::ExpressionList *expressionList;

    tree::Identity *identity;

    tree::Type *type;
    tree::Identifier *id;
    tree::FunctionPrototype *prototype;
    tree::State *state;

    /* The lexer returns these... */
    int integer;
    float real;
    char string[1024]; /* FIXME: We should make this bigger... */
}

/* Start tokens */
%token START_USE
%token START_INCLUDE

/* Language structure */
%token TOKEN_EOS
%token TOKEN_SEMICOLON
%token TOKEN_BRACKETS_OPEN
%token TOKEN_BRACKETS_CLOSE
%token TOKEN_PARENTHESIS_OPEN
%token TOKEN_PARENTHESIS_CLOSE
%token TOKEN_BRACE_OPEN
%token TOKEN_BRACE_CLOSE
%token TOKEN_COMMA
%token TOKEN_DIRECT_ACCESS
%token TOKEN_MESSAGE_ACCESS

/* Operators */
%token TOKEN_LOGICAL_OR
%token TOKEN_LOGICAL_AND
%token TOKEN_OR
%token TOKEN_XOR
%token TOKEN_AND
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_LE
%token TOKEN_LT
%token TOKEN_GE
%token TOKEN_GT
%token TOKEN_ADD
%token TOKEN_SUBTRACT
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_MODULUS
%token TOKEN_LOGICAL_NOT
%token TOKEN_NOT
%token TOKEN_CAST
%token TOKEN_EQUALS

/* Basic Types */
%token<integer> TOKEN_INTEGER
%token<real> TOKEN_FLOAT
%token<string> TOKEN_STRING

/* Keywords */
%token TOKEN_END
%token TOKEN_INCLUDE
%token TOKEN_USE
%token TOKEN_GLOBAL
%token TOKEN_SHARED
%token TOKEN_CONST
%token TOKEN_VAR
%token TOKEN_REF
%token TOKEN_FUNCTION
%token TOKEN_RETURN
%token TOKEN_STATE
%token TOKEN_RESET

/* Built-In Types */
%token TOKEN_TYPE_INT
%token TOKEN_TYPE_FLOAT
%token TOKEN_TYPE_STRING

/* Identifiers */
%token<string> TOKEN_NAME
%token<string> TOKEN_ID

/* Return types */
%type<use> use
%type<statementList> include
%type<statementList> o_program_includes
%type<statementList> program_includes
%type<statementList> include_statement
%type<statementList> o_program_cvrs
%type<statementList> program_cvrs
%type<statement> program_cvr
%type<statement> constant_statement
%type<expression> constant_assignment
%type<identity> constant
%type<statement> s_var_statement
%type<expression> s_var_assignment
%type<expression> s_variable
%type<statement> s_ref_statement
%type<expression> s_ref_assignment
%type<expression> s_reference
%type<statementList> o_program_functions
%type<statementList> program_functions
%type<statement> program_function
%type<expression> s_function_prototype
%type<prototype> function_prototype
%type<expressionList> o_arguments
%type<expressionList> arguments
%type<expression> argument
%type<state> function_state
%type<statementList> statements
%type<statement> statement
%type<statement> variable_statement
%type<expression> variable_assignment
%type<identity> variable
%type<statement> reference_statement
%type<expression> reference_assignment
%type<identity> reference
%type<statement> a_or_e_statement
%type<expression> assign_or_expression
%type<expression> assignment
%type<expression> expression
%type<expression> l_or_expression
%type<expression> l_and_expression
%type<expression> or_expression
%type<expression> xor_expression
%type<expression> and_expression
%type<expression> eq_expression
%type<expression> rel_expression
%type<expression> add_expression
%type<expression> mult_expression
%type<expression> cast_expression
%type<expression> unary_expression
%type<expression> access_expression
%type<expression> postfix_expression
%type<expression> array_expression
%type<expression> call_expression
%type<expressionList> argument_expressions
%type<expression> expression_atom
%type<type> type
%type<id> identifier
%type<id> name
%type<statement> return_statement
%type<statement> state_statement
%type<state> state

/* Start symbol */
%start start

%%

start               :   START_USE use
                        {
                            if($2)
                            {
                                data->uses->push_back($2);
                            }
                        }
                    |   START_INCLUDE include
                        {
                            data->statements = $2;
                        }
                    ;

use                 :   o_program_includes o_program_uses o_program_cvrs o_program_functions
                        {
                            tree::StatementList *useStatements = NULL;

                            // Check there is actually something in this scope...
                            // (Uses are not nested so we don't count them here...)
                            if($1 || $3 || $4)
                            {
                                useStatements = $1;

                                if($3)
                                {
                                    if(useStatements)
                                    {
                                        useStatements->insert(useStatements->end(), $3->begin(), $3->end());
                                        delete $3;
                                    }
                                    else
                                    {
                                        useStatements = $3;
                                    }
                                }

                                if($4)
                                {
                                    if(useStatements)
                                    {
                                        useStatements->insert(useStatements->end(), $4->begin(), $4->end());
                                        delete $4;
                                    }
                                    else
                                    {
                                        useStatements = $4;
                                    }
                                }
                            }

                            $$ = new tree::Use(useStatements);
                        }
                    ;

include             :   o_program_includes o_program_cvrs /* FIXME */
                        {
                            $$ = $1;

                            if($2)
                            {
                                if($$)
                                {
                                    $$->insert($$->end(), $2->begin(), $2->end());
                                    delete $2;
                                }
                                else
                                {
                                    $$ = $2;
                                }                                
                            }
                        }
                    ;

o_program_includes  :   /* Empty */
                        {
                            $$ = NULL;
                        }
                    |   program_includes
                        {
                            $$ = $1;
                        }
                    ;

program_includes    :   include_statement
                        {
                            $$ = $1;
                        }
                    |   program_includes include_statement
                        {
                            if($1)
                            {
                                $$ = $1;
                                $$->insert($$->end(), $2->begin(), $2->end());
                                delete $2;
                            }
                            else
                            {
                                $$ = $2;
                            }
                        }
                    ;

include_statement   :   TOKEN_INCLUDE TOKEN_ID TOKEN_EOS
                        {
                            char tmp[1024];

                            loader::includeNameToFilename(tmp, $2);

                            if(loader::includeFile(tmp, tmp))
                            {
                                if(!data->isParsedFile(tmp))
                                {
                                    FILE *input = fopen(tmp, "r");
                                    lexer::Data lexerData;
                                    void *currentLexer = data->lexer;

                                    lexerData.type = lexer::Data::TYPE_INCLUDE;
                                    lexerData.startSymbolIssued = false;

                                    yylex_init_extra(&lexerData, &data->lexer);
                                    yyset_in(input, data->lexer);

                                    data->addParsedFile(tmp);
                                    loader::pushCWD(dirname(tmp));

                                    yyparse(data);

                                    loader::popCWD();

                                    yylex_destroy(data->lexer);
                                    fclose(input);

                                    data->lexer = currentLexer;

                                    $$ = data->statements;
                                }
                            }
                            else
                            {
                                std::string error("Could not find include file ");
                                error += tmp;
                                error::enqueue(0, error.c_str()); // FIXME

                                $$ = NULL;
                            }
                        }
                    ;

o_program_uses      :   /* Empty */
                    |   program_uses
                    ;

program_uses        :   use_statement
                    |   program_uses use_statement
                    ;

use_statement       :   TOKEN_USE TOKEN_NAME TOKEN_EOS
                        {
                            char tmp[1024];

                            loader::useNameToFilename(tmp, $2);

                            if(loader::useFile(tmp, tmp))
                            {
                                if(!data->isParsedFile(tmp))
                                {
                                    FILE *input = fopen(tmp, "r");
                                    lexer::Data lexerData;
                                    void *currentLexer = data->lexer;

                                    lexerData.type = lexer::Data::TYPE_USE;
                                    lexerData.startSymbolIssued = false;

                                    yylex_init_extra(&lexerData, &data->lexer);
                                    yyset_in(input, data->lexer);

                                    data->addParsedFile(tmp);
                                    loader::pushCWD(dirname(tmp));

                                    yyparse(data);

                                    loader::popCWD();

                                    yylex_destroy(data->lexer);
                                    fclose(input);

                                    data->lexer = currentLexer;
                                }
                            }
                            else
                            {
                                std::string error("Could not find include file ");
                                error += tmp;
                                error::enqueue(0, error.c_str()); // FIXME
                            }
                        }
                    ;

o_program_cvrs      :   /* Empty */
                        {
                            $$ = NULL;
                        }
                    |   program_cvrs
                        {
                            $$ = $1;
                        }
                    ;

program_cvrs        :   program_cvr                                                                 /* CVRs = constants + variables + references */
                        {
                            $$ = new tree::StatementList();
                            $$->push_back($1);
                        }
                    |   program_cvrs program_cvr
                        {
                            $$ = $1;
                            $$->push_back($2);
                        }
                    ;

program_cvr         :   constant_statement
                        {
                            $$ = $1;
                        }
                    |   s_var_statement
                        {
                            $$ = $1;
                        }
                    |   s_ref_statement
                        {
                            $$ = $1;
                        }
                    ;

constant_statement  :   constant_assignment TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

constant_assignment :   constant TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

constant            :   TOKEN_CONST name
                        {
                            tree::Type *type = new tree::Int();
                            $$ = new tree::Constant(type, $2);
                        }
                    |   TOKEN_CONST type TOKEN_CAST name
                        {
                            $$ = new tree::Constant($2, $4);
                        }
                    ;

s_var_statement     :   s_var_assignment TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

s_var_assignment    :   s_variable
                        {
                            $$ = $1;
                        }
                    |   s_variable TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

s_variable          :   variable
                        {
                            $$ = $1;
                        }
                    |   TOKEN_GLOBAL variable
                        {
                            $$ = new tree::GlobalScoping($2);
                        }
                    |   TOKEN_SHARED variable
                        {
                            $$ = new tree::SharedScoping($2);
                        }
                    ;

s_ref_statement     :   s_ref_assignment TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

s_ref_assignment    :   s_reference
                        {
                            $$ = $1;
                        }
                    |   s_reference TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

s_reference         :   reference
                        {
                            $$ = $1;
                        }
                    |   TOKEN_GLOBAL reference
                        {
                            $$ = new tree::GlobalScoping($2);
                        }
                    |   TOKEN_SHARED reference
                        {
                            $$ = new tree::SharedScoping($2);
                        }
                    ;

o_program_functions :   /* Empty */
                        {
                            $$ = NULL;
                        }
                    |   program_functions
                        {
                            $$ = $1;
                        }
                    ;

program_functions   :   program_function
                        {
                            $$ = new tree::StatementList();
                            $$->push_back($1);
                        }
                    |   program_functions program_function
                        {
                            $$ = $1;
                            $$->push_back($2);
                        }
                    ;

program_function    :   s_function_prototype function_state TOKEN_EOS statements TOKEN_END TOKEN_EOS /* FIXME, support states */
                        {
                            $$ = new tree::Function($1, $4);
                        }
                    ;

s_function_prototype:   function_prototype
                        {
                            $$ = $1;
                        }
                    |   TOKEN_GLOBAL function_prototype
                        {
                            $$ = new tree::GlobalScoping($2);
                        }
                    |   TOKEN_SHARED function_prototype
                        {
                            $$ = new tree::SharedScoping($2);
                        }
                    ;

function_prototype  :   TOKEN_FUNCTION identifier TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                        {
                            tree::Type *type = new tree::Int();
                            $$ = new tree::FunctionPrototype(type, $2, $4);
                        }
                    |   TOKEN_FUNCTION type TOKEN_CAST identifier TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                        {
                            $$ = new tree::FunctionPrototype($2, $4, $6);
                        }
                    ;

o_arguments         :   /* Empty */
                        {
                            $$ = NULL;
                        }
                    |   arguments
                        {
                            $$ = $1;
                        }
                    ;

arguments           :   argument
                        {
                            $$ = new tree::ExpressionList();
                            $$->push_back($1);
                        }
                    |   arguments TOKEN_COMMA argument
                        {
                            $$ = $1;
                            $$->push_back($3);
                        }
                    ;

argument            :   identifier
                        {
                            tree::Type *type = new tree::Int();
                            $$ = new tree::Variable(type, $1);
                        }
                    |   type TOKEN_CAST identifier
                        {
                            $$ = new tree::Variable($1, $3);
                        }
                    |   variable
                        {
                            $$ = $1;
                        }
                    |   reference
                        {
                            $$ = $1;
                        }
                    ;

function_state      :   /* No state */
                        {
                            $$ = NULL;
                        }
                    |   TOKEN_LT state TOKEN_GT
                        {
                            $$ = $2;
                        }
                    ;

statements          :   statement
                        {
                            $$ = new tree::StatementList();
                            $$->push_back($1);
                        }
                    |   statements statement
                        {
                            $$ = $1;
                            $$->push_back($2);
                        }
                    ;

statement           :   variable_statement
                        {
                            $$ = $1;
                        }
                    |   reference_statement
                        {
                            $$ = $1;
                        }
                    |   a_or_e_statement
                        {
                            $$ = $1;
                        }
                    |   return_statement
                        {
                            $$ = $1;
                        }
                    |   state_statement
                        {
                            $$ = $1;
                        }
                    ;

variable_statement  :   variable_assignment TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

variable_assignment :   variable
                        {
                            $$ = $1;
                        }
                    |   variable TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

variable            :   TOKEN_VAR identifier
                        {
                            tree::Type *type = new tree::Int();
                            $$ = new tree::Variable(type, $2);
                        }
                    |   TOKEN_VAR type TOKEN_CAST identifier
                        {
                            $$ = new tree::Variable($2, $4);
                        }
                    ;

reference_statement :   reference_assignment TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

reference_assignment:   reference
                        {
                            $$ = $1;
                        }
                    |   reference TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

reference           :   TOKEN_REF identifier
                        {
                            tree::Type *type = new tree::Int();
                            $$ = new tree::Reference(type, $2);
                        }
                    |   TOKEN_REF type TOKEN_CAST identifier
                        {
                            $$ = new tree::Reference($2, $4);
                        }
                    ;

a_or_e_statement    :   assign_or_expression TOKEN_EOS
                        {
                            $$ = new tree::Execute($1);
                        }
                    ;

assign_or_expression:   assignment
                        {
                            $$ = $1;
                        }
                    |   expression
                        {
                            $$ = $1;
                        }

                    |   error                                                                       /* Special case!! Checking here will catch a lot of cases, should produce better error reports */
                        {
                            $$ = NULL; // FIXME
                        }
                    ;

assignment          :   identifier TOKEN_EQUALS expression
                        {
                            $$ = new tree::Assign($1, $3);
                        }
                    ;

expression          :   l_or_expression
                        {
                            $$ = $1;
                        }
                    ;

l_or_expression     :   l_and_expression
                        {
                            $$ = $1;
                        }
                    |   l_or_expression TOKEN_LOGICAL_OR l_and_expression
                        {
                            $$ = new tree::LogicalOr($1, $3);
                        }
                    ;

l_and_expression    :   or_expression
                        {
                            $$ = $1;
                        }
                    |   l_and_expression TOKEN_LOGICAL_AND or_expression
                        {
                            $$ = new tree::LogicalAnd($1, $3);
                        }
                    ;

or_expression       :   xor_expression
                        {
                            $$ = $1;
                        }
                    |   or_expression TOKEN_OR xor_expression
                        {
                            $$ = new tree::Or($1, $3);
                        }
                    ;

xor_expression      :   and_expression
                        {
                            $$ = $1;
                        }
                    |   xor_expression TOKEN_XOR and_expression
                        {
                            $$ = new tree::Xor($1, $3);
                        }
                    ;

and_expression      :   eq_expression
                        {
                            $$ = $1;
                        }
                    |   and_expression TOKEN_AND eq_expression
                        {
                            $$ = new tree::And($1, $3);
                        }
                    ;

eq_expression       :   rel_expression
                        {
                            $$ = $1;
                        }
                    |   eq_expression TOKEN_EQ rel_expression
                        {
                            $$ = new tree::Equal($1, $3);
                        }
                    |   eq_expression TOKEN_NE rel_expression
                        {
                            $$ = new tree::Unequal($1, $3);
                        }
                    ;

rel_expression      :   add_expression
                        {
                            $$ = $1;
                        }
                    |   rel_expression TOKEN_LT add_expression
                        {
                            $$ = new tree::LessThan($1, $3);
                        }
                    |   rel_expression TOKEN_LE add_expression
                        {
                            $$ = new tree::LessEqual($1, $3);
                        }
                    |   rel_expression TOKEN_GT add_expression
                        {
                            $$ = new tree::GreaterThan($1, $3);
                        }
                    |   rel_expression TOKEN_GE add_expression
                        {
                            $$ = new tree::GreaterEqual($1, $3);
                        }
                    ;

add_expression      :   mult_expression
                        {
                            $$ = $1;
                        }
                    |   add_expression TOKEN_ADD mult_expression
                        {
                            $$ = new tree::Add($1, $3);
                        }
                    |   add_expression TOKEN_SUBTRACT mult_expression
                        {
                            $$ = new tree::Subtract($1, $3);
                        }
                    ;

mult_expression     :   cast_expression
                        {
                            $$ = $1;
                        }
                    |   mult_expression TOKEN_MULTIPLY cast_expression
                        {
                            $$ = new tree::Multiply($1, $3);
                        }
                    |   mult_expression TOKEN_DIVIDE cast_expression
                        {
                            $$ = new tree::Divide($1, $3);
                        }
                    |   mult_expression TOKEN_MODULUS cast_expression
                        {
                            $$ = new tree::Modulus($1, $3);
                        }
                    ;

cast_expression     :   unary_expression
                        {
                            $$ = $1;
                        }
                    |   type TOKEN_CAST expression_atom
                        {
                            $$ = new tree::Cast($1, $3);
                        }
                    ;

unary_expression    :   access_expression
                        {
                            $$ = $1;
                        }
                    |   TOKEN_SUBTRACT access_expression
                        {
                            $$ = new tree::Minus($2);
                        }
                    |   TOKEN_LOGICAL_NOT access_expression
                        {
                            $$ = new tree::LogicalNot($2);
                        }
                    |   TOKEN_NOT access_expression
                        {
                            $$ = new tree::Not($2);
                        }
                    ;

access_expression   :   postfix_expression
                        {
                            $$ = $1;
                        }
                    |   postfix_expression TOKEN_DIRECT_ACCESS postfix_expression
                        {
                            $$ = new tree::DirectAccess($1, $3);
                        }
                    |   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression
                        {
                            $$ = new tree::MessageAccess($1, $3);
                        }
                    ;

postfix_expression  :   expression_atom
                        {
                            $$ = $1;
                        }
                    |   array_expression
                        {
                            $$ = $1;
                        }
                    |   call_expression
                        {
                            $$ = $1;
                        }
                    ;

array_expression    :   postfix_expression TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                        {
                            $$ = new tree::ArrayAccess($1, $3);
                        }
                    ;

call_expression     :   identifier TOKEN_PARENTHESIS_OPEN TOKEN_PARENTHESIS_CLOSE
                        {
                            $$ = new tree::FunctionCall($1);
                        }
                    |   identifier TOKEN_PARENTHESIS_OPEN argument_expressions TOKEN_PARENTHESIS_CLOSE
                        {
                            $$ = new tree::FunctionCall($1, $3);
                        }
                    ;

argument_expressions:   expression
                        {
                            $$ = new tree::ExpressionList();
                            $$->push_back($1);
                        }
                    |   argument_expressions TOKEN_COMMA expression
                        {
                            $$ = $1;
                            $$->push_back($3);
                        }
                    ;

expression_atom     :   name /* Constant */
                        {
                            $$ = $1;
                        }
                    |   TOKEN_INTEGER
                        {
                            $$ = new tree::IntLiteral($1);
                        }
                    |   TOKEN_FLOAT
                        {
                            $$ = new tree::FloatLiteral($1);
                        }
                    |   TOKEN_STRING
                        {
                            $$ = new tree::StringLiteral(std::string($1));
                        }
                    |   identifier
                        {
                            $$ = $1;
                        }
                    |   TOKEN_PARENTHESIS_OPEN expression TOKEN_PARENTHESIS_CLOSE
                        {
                            $$ = $2;
                        }
                    ;

type                :   TOKEN_TYPE_INT
                        {
                            $$ = new tree::Int();
                        }
                    |   TOKEN_TYPE_FLOAT
                        {
                            $$ = new tree::Float();
                        }
                    |   TOKEN_TYPE_STRING
                        {
                            $$ = new tree::String();
                        }
                    |   TOKEN_TYPE_STRING TOKEN_LT TOKEN_INTEGER TOKEN_GT
                        {
                            $$ = new tree::String($3);
                        }

                        
                    /*|   TOKEN_NAME
                        {
                            $$ = new tree::UDT($1);                       // UDTs force this class to be needed?? FIXME
                        }*/
                    ;

identifier          :   TOKEN_ID
                        {
                            $$ = new tree::Identifier(std::string($1));
                        }
                    ;

name                :   TOKEN_NAME
                        {
                            $$ = new tree::Identifier(std::string($1));
                        }
                    ;

return_statement    :   TOKEN_RETURN expression TOKEN_EOS
                        {
                            $$ = new tree::Return($2);
                        }
                    ;

state_statement     :   TOKEN_STATE state TOKEN_EOS
                        {
                            $$ = new tree::SetState($2);
                        }
                    ;

state               :   TOKEN_RESET
                        {
                            $$ = new tree::DefaultState();
                        }
                    |   TOKEN_NAME
                        {
                            $$ = new tree::NamedState($1);
                        }
                    ;

%%

void yyerror(YYLTYPE *locp, void *lexer, const char *error)
{
    error::enqueue(locp->first_line, error);
}

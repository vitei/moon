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
    #include "compiler/tree.h"

    /* Generated headers */
    #include "generated/parser.h"
    #include "generated/lexer.h"

    #define YYLTYPE tree::Node::Location
    #define scanner data->lexer

    void yyerror(YYLTYPE *locp, void *lexer, const char *error);

    #define YYLLOC_DEFAULT(Current, Rhs, N) \
        do \
        { \
            if(YYID(N)) \
            { \
                (Current).filename = YYRHSLOC(Rhs, 1).filename; \
                (Current).start = YYRHSLOC(Rhs, 1).start; \
                (Current).end = YYRHSLOC(Rhs, N).end; \
            } \
            else \
            { \
                (Current).filename = (Current).filename; \
                (Current).start = (Current).end = YYRHSLOC(Rhs, 0).end; \
            } \
        } \
        while(YYID(0));
%}

/* GLR */
%glr-parser

/* Re-entrant */
%pure-parser
%parse-param {parser::Data *data}
%lex-param {void *scanner}

/* Program locations */
%locations

%initial-action
{
    ASSERT(data->currentFilename);
    @$.filename = data->currentFilename;
};

/* Better errors!! */
%error-verbose

/*  */
%union {
    tree::Use *use;

    tree::Statement *statement;
    tree::Statements *statements;

    tree::Expression *expression;
    tree::Expressions *expressions;

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
%token TOKEN_OR_EQUALS
%token TOKEN_XOR_EQUALS
%token TOKEN_AND_EQUALS
%token TOKEN_ADD_EQUALS
%token TOKEN_SUBTRACT_EQUALS
%token TOKEN_MULTIPLY_EQUALS
%token TOKEN_DIVIDE_EQUALS
%token TOKEN_MODULUS_EQUALS
%token TOKEN_CAST
%token TOKEN_EQUALS

/* Basic Types */
%token<integer> TOKEN_INTEGER
%token<real> TOKEN_FLOAT
%token<string> TOKEN_STRING

/* Keywords */
%token TOKEN_NULL
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_END
%token TOKEN_INCLUDE
%token TOKEN_USE
%token TOKEN_IMPORT
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
%token TOKEN_TYPE_BOOL
%token TOKEN_TYPE_INT
%token TOKEN_TYPE_FLOAT
%token TOKEN_TYPE_STRING

/* Identifiers */
%token<string> TOKEN_NAME
%token<string> TOKEN_ID

/* Return types */
%type<use> use
%type<statements> include
%type<statements> o_program_includes
%type<statements> program_includes
%type<statements> include_statement
%type<statements> o_imports
%type<statements> imports
%type<statement> import_statement
%type<statements> o_program_cvrs
%type<statements> program_cvrs
%type<statement> program_cvr
%type<statement> s_constant_statement
%type<statement> constant_statement
%type<expression> constant_assignment
%type<identity> constant
%type<statement> s_variable_statement
%type<statement> s_reference_statement
%type<statements> o_program_functions
%type<statements> program_functions
%type<statement> s_program_function
%type<statement> program_function
%type<prototype> function_prototype
%type<expressions> o_arguments
%type<expressions> arguments
%type<expression> argument
%type<state> function_state
%type<statements> statements
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
%type<expressions> o_argument_expressions
%type<expressions> argument_expressions
%type<expression> expression_atom
%type<type> cv_type
%type<type> type
%type<id> identifier
%type<id> name
%type<statement> return_statement
%type<statement> state_statement
%type<state> state

/* Start symbol */
%start start

%%

start                   :   START_USE use
                            {
                                data->uses->push_back($2);
                            }
                        |   START_INCLUDE include
                            {
                                data->statements = $2;
                            }
                        ;

use                     :   o_program_includes o_program_uses o_imports o_program_cvrs o_program_functions
                            {
                                tree::Statements *useStatements = NULL;

                                // Check there is actually something in this scope...
                                // (Uses are not nested so we don't count them here...)
                                if($1 || $3 || $4 || $5)
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

                                    if($5)
                                    {
                                        if(useStatements)
                                        {
                                            useStatements->insert(useStatements->end(), $5->begin(), $5->end());
                                            delete $5;
                                        }
                                        else
                                        {
                                            useStatements = $5;
                                        }
                                    }
                                }

                                $$ = new tree::Use(data->currentName, useStatements);
                            }
                        ;

include                 :   o_program_includes o_imports o_program_cvrs /* FIXME */
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

                                if($3)
                                {
                                    if($$)
                                    {
                                        $$->insert($$->end(), $3->begin(), $3->end());
                                        delete $3;
                                    }
                                    else
                                    {
                                        $$ = $3;
                                    }                                
                                }
                            }
                        ;

o_program_includes      :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   program_includes
                            {
                                $$ = $1;
                            }
                        ;

program_includes        :   include_statement
                            {
                                $$ = $1;
                            }
                        |   program_includes include_statement
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

include_statement       :   TOKEN_INCLUDE TOKEN_ID TOKEN_EOS
                            {
                                $$ = NULL;

                                char tmp[1024];

                                loader::includeNameToFilename(tmp, $2);

                                if(loader::includeFile(tmp, tmp))
                                {
                                    std::string filename = tmp;

                                    if(!data->isParsedFile(filename))
                                    {
                                        loader::pushCWD(dirname(tmp));
                                        data->parseInclude(filename);
                                        loader::popCWD();

                                        $$ = data->statements;
                                    }
                                }
                                else
                                {
                                    std::string error("Could not find include file ");
                                    error += tmp;
                                    error::enqueue(@1, error.c_str());
                                }
                            }
                        ;

o_program_uses          :   /* Empty */
                        |   program_uses
                        ;

program_uses            :   use_statement
                        |   program_uses use_statement
                        ;

use_statement           :   TOKEN_USE TOKEN_NAME TOKEN_EOS
                            {
                                char tmp[1024];

                                loader::useNameToFilename(tmp, $2);

                                if(loader::useFile(tmp, tmp))
                                {
                                    std::string filename = tmp;

                                    if(!data->isParsedFile(filename))
                                    {
                                        loader::pushCWD(dirname(tmp));
                                        data->parseUse($2, filename);
                                        loader::popCWD();
                                    }
                                }
                                else
                                {
                                    std::string error("Could not find use file ");
                                    error += tmp;
                                    error::enqueue(@1, error.c_str());
                                }
                            }
                        ;

o_imports               :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   imports
                            {
                                $$ = $1;
                            }
                        ;

imports                 :   import_statement
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   imports import_statement
                            {
                                $$ = $1;

                                if($2)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Statements();
                                    }
                                    $$->push_back($2);
                                }
                            }
                        ;

import_statement        :   TOKEN_IMPORT function_prototype TOKEN_EOS
                            {
                                $$ = new tree::Import($2);
                                $$->setLocation(@1);
                            }
                        ;

o_program_cvrs          :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   program_cvr
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   program_cvr program_cvrs
                            {
                                $$ = $2;

                                if($1)
                                {
                                    if($$)
                                    {
                                        $$->push_front($1);
                                    }
                                    else
                                    {
                                        $$ = new tree::Statements();
                                        $$->push_back($1);
                                    }
                                }
                            }
                        ;

program_cvrs            :   program_cvr                                                                 /* CVRs = constants + variables + references */
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   program_cvrs program_cvr
                            {
                                $$ = $1;

                                if($2)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Statements();
                                    }
                                    $$->push_back($2);
                                }
                            }
                        ;

program_cvr             :   s_constant_statement
                            {
                                $$ = $1;
                            }
                        |   s_variable_statement
                            {
                                $$ = $1;
                            }
                        |   s_reference_statement
                            {
                                $$ = $1;
                            }
                        ;

s_constant_statement    :   constant_statement
                            {
                                $$ = $1;
                            }
                            /* FIXME, what to do about global scoping?? */
                        |   TOKEN_SHARED constant_statement
                            {
                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

constant_statement      :   constant_assignment TOKEN_EOS
                            {
                                $$ = new tree::Execute($1);
                                $$->setLocation(@1);
                            }
                        ;

constant_assignment     :   constant TOKEN_EQUALS expression
                            {
                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

constant                :   TOKEN_CONST TOKEN_NAME
                            {
                                tree::Type *type = new tree::Int();
                                type->setLocation(@1);

                                $$ = new tree::Constant(type, std::string($2));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_CONST cv_type TOKEN_CAST TOKEN_NAME
                            {
                                $$ = new tree::Constant($2, std::string($4));
                                $$->setLocation(@1);
                            }
                        ;

s_variable_statement    :   variable_statement
                            {
                                $$ = $1;
                            }
                        |   TOKEN_GLOBAL variable TOKEN_EOS
                            {
                                tree::Execute *execute = new tree::Execute($2);
                                execute->setLocation(@2);

                                $$ = new tree::GlobalScoping(execute);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_SHARED variable_statement
                            {
                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

s_reference_statement   :   reference_statement
                            {
                                $$ = $1;
                            }
                        |   TOKEN_GLOBAL reference TOKEN_EOS
                            {
                                tree::Execute *execute = new tree::Execute($2);
                                execute->setLocation(@2);

                                $$ = new tree::GlobalScoping(execute);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_SHARED reference_statement
                            {
                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

o_program_functions     :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   s_program_function
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   s_program_function program_functions
                            {
                                $$ = $2;

                                if($1)
                                {
                                    if($$)
                                    {
                                        $$->push_front($1);
                                    }
                                    else
                                    {
                                        $$ = new tree::Statements();
                                        $$->push_back($1);
                                    }
                                }
                            }
                        ;

program_functions       :   s_program_function
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   program_functions s_program_function
                            {
                                $$ = $1;

                                if($2)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Statements();
                                    }
                                    $$->push_back($2);
                                }
                            }
                        ;

s_program_function      :   program_function
                            {
                                $$ = $1;
                            }
                            /* FIXME, what to do about global scoping?? */
                        |   TOKEN_SHARED program_function
                            {
                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

program_function        :   function_prototype function_state TOKEN_EOS statements TOKEN_END TOKEN_EOS /* FIXME, support states */
                            {
                                $$ = new tree::Function($1, $4);
                                $$->setLocation(@1);
                            }
                        ;

function_prototype      :   TOKEN_FUNCTION TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                            {
                                tree::Type *type = new tree::Int();
                                type->setLocation(@1);

                                $$ = new tree::FunctionPrototype(type, std::string($2), $4);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_FUNCTION type TOKEN_CAST TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                            {
                                $$ = new tree::FunctionPrototype($2, std::string($4), $6);
                                $$->setLocation(@1);
                            }
                        ;

o_arguments             :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   arguments
                            {
                                $$ = $1;
                            }
                        ;

arguments               :   argument
                            {
                                if($1)
                                {
                                    $$ = new tree::Expressions();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   arguments TOKEN_COMMA argument
                            {
                                $$ = $1;

                                if($3)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Expressions();
                                    }
                                    $$->push_back($3);
                                }
                            }
                        ;

argument                :   TOKEN_ID
                            {
                                tree::Type *type = new tree::Int();
                                type->setLocation(@1);

                                $$ = new tree::Variable(type, std::string($1));
                                $$->setLocation(@1);
                            }
                        |   cv_type TOKEN_CAST TOKEN_ID
                            {
                                $$ = new tree::Variable($1, std::string($3));
                                $$->setLocation(@1);
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

function_state          :   /* No state */
                            {
                                $$ = NULL;
                            }
                        |   TOKEN_LT state TOKEN_GT
                            {
                                $$ = $2;
                            }
                        ;

statements              :   statement
                            {
                                if($1)
                                {
                                    $$ = new tree::Statements();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   statements statement
                            {
                                $$ = $1;

                                if($2)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Statements();
                                    }
                                    $$->push_back($2);
                                }
                            }
                        ;

statement               :   variable_statement
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

variable_statement      :   variable_assignment TOKEN_EOS
                            {
                                $$ = new tree::Execute($1);
                                $$->setLocation(@1);
                            }
                        ;

variable_assignment     :   variable
                            {
                                $$ = $1;
                            }
                        |   variable TOKEN_EQUALS expression
                            {
                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

variable                :   TOKEN_VAR TOKEN_ID
                            {
                                tree::Type *type = new tree::Int();
                                type->setLocation(@1);

                                $$ = new tree::Variable(type, std::string($2));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_VAR cv_type TOKEN_CAST TOKEN_ID
                            {
                                $$ = new tree::Variable($2, std::string($4));
                                $$->setLocation(@1);
                            }
                        ;

reference_statement     :   reference_assignment TOKEN_EOS
                            {
                                $$ = new tree::Execute($1);
                                $$->setLocation(@1);
                            }
                        ;

reference_assignment    :   reference
                            {
                                $$ = $1;
                            }
                        |   reference TOKEN_EQUALS expression
                            {
                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

reference               :   TOKEN_REF TOKEN_ID
                            {
                                tree::Type *type = new tree::Int();
                                type->setLocation(@1);

                                $$ = new tree::Reference(type, std::string($2));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_REF type TOKEN_CAST TOKEN_ID
                            {
                                $$ = new tree::Reference($2, std::string($4));
                                $$->setLocation(@1);
                            }
                        ;

a_or_e_statement        :   assign_or_expression TOKEN_EOS
                            {
                                if($1)
                                {
                                    $$ = new tree::Execute($1);
                                    $$->setLocation(@1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        ;

assign_or_expression    :   assignment
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

assignment              :   identifier TOKEN_EQUALS expression
                            {
                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_OR_EQUALS expression
                            {
                                tree::Or *orExpression = new tree::Or($1, $3);
                                orExpression->setLocation(@2);

                                $$ = new tree::Assign($1, orExpression);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_XOR_EQUALS expression
                            {
                                tree::Xor *xorExpression = new tree::Xor($1, $3);
                                xorExpression->setLocation(@2);

                                $$ = new tree::Assign($1, xorExpression);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_AND_EQUALS expression
                            {
                                tree::And *andExpression = new tree::And($1, $3);
                                andExpression->setLocation(@2);

                                $$ = new tree::Assign($1, andExpression);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_ADD_EQUALS expression
                            {
                                tree::Add *add = new tree::Add($1, $3);
                                add->setLocation(@2);

                                $$ = new tree::Assign($1, add);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_SUBTRACT_EQUALS expression
                            {
                                tree::Subtract *subtract = new tree::Subtract($1, $3);
                                subtract->setLocation(@2);

                                $$ = new tree::Assign($1, subtract);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_MULTIPLY_EQUALS expression
                            {
                                tree::Multiply *multiply = new tree::Multiply($1, $3);
                                multiply->setLocation(@2);

                                $$ = new tree::Assign($1, multiply);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_DIVIDE_EQUALS expression
                            {
                                tree::Divide *divide = new tree::Divide($1, $3);
                                divide->setLocation(@2);

                                $$ = new tree::Assign($1, divide);
                                $$->setLocation(@2);
                            }
                        |   identifier TOKEN_MODULUS_EQUALS expression
                            {
                                tree::Modulus *modulus = new tree::Modulus($1, $3);
                                modulus->setLocation(@2);

                                $$ = new tree::Assign($1, modulus);
                                $$->setLocation(@2);
                            }
                        ;

expression              :   l_or_expression
                            {
                                $$ = $1;
                            }
                        ;

l_or_expression         :   l_and_expression
                            {
                                $$ = $1;
                            }
                        |   l_or_expression TOKEN_LOGICAL_OR l_and_expression
                            {
                                $$ = new tree::LogicalOr($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

l_and_expression        :   or_expression
                            {
                                $$ = $1;
                            }
                        |   l_and_expression TOKEN_LOGICAL_AND or_expression
                            {
                                $$ = new tree::LogicalAnd($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

or_expression           :   xor_expression
                            {
                                $$ = $1;
                            }
                        |   or_expression TOKEN_OR xor_expression
                            {
                                $$ = new tree::Or($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

xor_expression          :   and_expression
                            {
                                $$ = $1;
                            }
                        |   xor_expression TOKEN_XOR and_expression
                            {
                                $$ = new tree::Xor($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

and_expression          :   eq_expression
                            {
                                $$ = $1;
                            }
                        |   and_expression TOKEN_AND eq_expression
                            {
                                $$ = new tree::And($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

eq_expression           :   rel_expression
                            {
                                $$ = $1;
                            }
                        |   eq_expression TOKEN_EQ rel_expression
                            {
                                $$ = new tree::Equal($1, $3);
                                $$->setLocation(@1);
                            }
                        |   eq_expression TOKEN_NE rel_expression
                            {
                                $$ = new tree::Unequal($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

rel_expression          :   add_expression
                            {
                                $$ = $1;
                            }
                        |   rel_expression TOKEN_LT add_expression
                            {
                                $$ = new tree::LessThan($1, $3);
                                $$->setLocation(@1);
                            }
                        |   rel_expression TOKEN_LE add_expression
                            {
                                $$ = new tree::LessEqual($1, $3);
                                $$->setLocation(@1);
                            }
                        |   rel_expression TOKEN_GT add_expression
                            {
                                $$ = new tree::GreaterThan($1, $3);
                                $$->setLocation(@1);
                            }
                        |   rel_expression TOKEN_GE add_expression
                            {
                                $$ = new tree::GreaterEqual($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

add_expression          :   mult_expression
                            {
                                $$ = $1;
                            }
                        |   add_expression TOKEN_ADD mult_expression
                            {
                                $$ = new tree::Add($1, $3);
                                $$->setLocation(@1);
                            }
                        |   add_expression TOKEN_SUBTRACT mult_expression
                            {
                                $$ = new tree::Subtract($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

mult_expression         :   cast_expression
                            {
                                $$ = $1;
                            }
                        |   mult_expression TOKEN_MULTIPLY cast_expression
                            {
                                $$ = new tree::Multiply($1, $3);
                                $$->setLocation(@1);
                            }
                        |   mult_expression TOKEN_DIVIDE cast_expression
                            {
                                $$ = new tree::Divide($1, $3);
                                $$->setLocation(@1);
                            }
                        |   mult_expression TOKEN_MODULUS cast_expression
                            {
                                $$ = new tree::Modulus($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

cast_expression         :   unary_expression
                            {
                                $$ = $1;
                            }
                        |   type TOKEN_CAST expression_atom
                            {
                                $$ = new tree::Cast($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

unary_expression        :   access_expression
                            {
                                $$ = $1;
                            }
                        |   TOKEN_SUBTRACT access_expression
                            {
                                $$ = new tree::Minus($2);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_LOGICAL_NOT access_expression
                            {
                                $$ = new tree::LogicalNot($2);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_NOT access_expression
                            {
                                $$ = new tree::Not($2);
                                $$->setLocation(@1);
                            }
                        ;

access_expression       :   postfix_expression
                            {
                                $$ = $1;
                            }
                        /*| postfix_expression TOKEN_DIRECT_ACCESS postfix_expression
                            {
                                $$ = new tree::DirectAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        |   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression
                            {
                                $$ = new tree::MessageAccess($1, $3);
                                $$->setLocation(@1);
                            }*/
                        ;

postfix_expression      :   expression_atom
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

array_expression        :   postfix_expression TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                            {
                                $$ = new tree::ArrayAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

call_expression         :   identifier TOKEN_PARENTHESIS_OPEN o_argument_expressions TOKEN_PARENTHESIS_CLOSE
                            {
                                $$ = new tree::FunctionCall($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

o_argument_expressions  :   /* Empty */
                            {
                                $$ = NULL;
                            }
                        |   argument_expressions
                            {
                                $$ = $1;
                            }

argument_expressions    :   expression
                            {
                                if($1)
                                {
                                    $$ = new tree::Expressions();
                                    $$->push_back($1);
                                }
                                else
                                {
                                    $$ = NULL;
                                }
                            }
                        |   argument_expressions TOKEN_COMMA expression
                            {
                                $$ = $1;

                                if($3)
                                {
                                    if(!$$)
                                    {
                                        $$ = new tree::Expressions();
                                    }
                                    $$->push_back($3);
                                }
                            }
                        ;

expression_atom         :   name /* Constant */
                            {
                                $$ = $1;
                            }
                        |   TOKEN_NULL
                            {
                                $$ = new tree::NullReference();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TRUE
                            {
                                $$ = new tree::BoolLiteral(true);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_FALSE
                            {
                                $$ = new tree::BoolLiteral(false);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_INTEGER
                            {
                                $$ = new tree::IntLiteral($1);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_FLOAT
                            {
                                $$ = new tree::FloatLiteral($1);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_STRING
                            {
                                $$ = new tree::StringLiteral(std::string($1));
                                $$->setLocation(@1);
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

cv_type                 :   type
                            {
                                $$ = $1;
                            }
                        |   TOKEN_TYPE_STRING TOKEN_LT TOKEN_INTEGER TOKEN_GT
                            {
                                $$ = new tree::String($3);
                                $$->setLocation(@1);
                            }
                        ;

type                    :   TOKEN_TYPE_BOOL
                            {
                                $$ = new tree::Bool();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_INT
                            {
                                $$ = new tree::Int();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_FLOAT
                            {
                                $$ = new tree::Float();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_STRING
                            {
                                $$ = new tree::String();
                                $$->setLocation(@1);
                            }

                        /*|   TOKEN_NAME
                            {
                                $$ = new tree::UDT($1);                       // UDTs force this class to be needed?? FIXME
                                $$->setLocation(@1);
                            }*/
                        ;

identifier              :   TOKEN_ID
                            {
                                $$ = new tree::Identifier(std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

name                    :   TOKEN_NAME
                            {
                                $$ = new tree::Identifier(std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

return_statement        :   TOKEN_RETURN expression TOKEN_EOS
                            {
                                $$ = new tree::Return($2);
                                $$->setLocation(@1);
                            }
                        ;

state_statement         :   TOKEN_STATE state TOKEN_EOS
                            {
                                $$ = new tree::SetState($2);
                                $$->setLocation(@1);
                            }
                        ;

state                   :   TOKEN_RESET
                            {
                                $$ = new tree::DefaultState();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_NAME
                            {
                                $$ = new tree::NamedState($1);
                                $$->setLocation(@1);
                            }
                        ;

%%

void yyerror(YYLTYPE *locp, void *lexer, const char *error)
{
    error::enqueue(*locp, error);
}

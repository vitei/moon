/*
 * moon.y
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include "common.h"

    #include <cstdio>
    #include <libgen.h>
    #include "compiler/error.h"
    #include "compiler/lexer.h"
    #include "compiler/loader.h"
    #include "compiler/parser.h"
    #include "compiler/tree.h"

    #define YYLTYPE tree::Node::Location
    #define scanner data->lexer

    void yyerror(YYLTYPE *locp, void *lexer, const char *error);

    #define YYLLOC_DEFAULT(Current, Rhs, N) \
        do \
        { \
            if(N) \
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
        while(0);
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
    @$.reset();
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
    tree::Variable *variable;
    tree::FunctionPrototype *prototype;

	tree::Member *member;
	tree::Members *members;
    /*tree::State *state;*/

    /* The lexer returns these... */
    long int integer;
    float real;
    char string[1024]; /* FIXME: We should make this bigger... */
}

%code {
    /* Nasty way to make this work with Bison 2.5 */
    extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param , void *yyscanner);
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
%token TOKEN_RANGE
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
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_END
%token TOKEN_INCLUDE
%token TOKEN_USE
%token TOKEN_IMPORT
%token TOKEN_GLOBAL
%token TOKEN_SHARED
%token TOKEN_CONST
%token TOKEN_DEF
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_IN
%token TOKEN_WHILE
%token TOKEN_RETURN
/*%token TOKEN_STATE*/
/*%token TOKEN_RESET*/

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
%type<statements> o_defines
%type<statements> defines
%type<statement> define
%type<statement> s_type_definition
%type<statement> type_definition
%type<members> type_members
%type<member> type_member
%type<statement> import_statement
%type<statement> s_constant_statement
%type<statement> constant_statement
%type<expression> constant_assignment
%type<identity> constant
%type<statements> o_declarations
%type<statements> declarations
%type<statement> declaration
%type<statement> s_variable_statement
%type<statements> o_callables
%type<statements> callables
%type<statement> callable
%type<statement> s_function
%type<statement> function
%type<statement> method
%type<prototype> prototype
%type<expressions> o_arguments
%type<expressions> arguments
%type<variable> argument
/*%type<state> function_state*/
%type<statements> o_statements
%type<statements> statements
%type<statement> statement
%type<statement> variable_statement
%type<expression> variable_assignment
%type<variable> variable
%type<statement> conditional_statement
%type<statement> loop_statement
%type<statement> single_statement
%type<statement> execute_statement
%type<expression> assign_or_function
%type<expression> assignment
%type<expression> access_assignee
%type<expression> assignee
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
%type<expression> access_call_expression
%type<expression> call_expression
%type<expressions> o_argument_expressions
%type<expressions> argument_expressions
%type<expression> expression_atom
%type<expression> conditional_expression
%type<type> type
%type<id> identifier
%type<id> name
%type<statement> block_statement
%type<statement> if_statement
%type<statement> for_statement
%type<statement> while_statement
%type<statement> return_statement
/*%type<statement> state_statement*/
/*%type<state> state*/

/* Start symbol */
%start start

%%

start                   :   START_USE use
                            {
                                LOG("start                   :   START_USE use");

                                data->uses->push_back($2);
                            }
                        |   START_INCLUDE include
                            {
                                LOG("start                   :   START_INCLUDE include");

                                data->statements = $2;
                            }
                        ;

use                     :   o_program_includes o_program_uses o_declarations o_callables
                            {
                                LOG("use                     :   o_program_includes o_program_uses o_declarations o_callables");

                                tree::Statements *useStatements = NULL;

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

                                $$ = new tree::Use(data->currentName, useStatements);
                            }
                        ;

include                 :   o_program_includes o_defines
                            {
                                LOG("include                 :   o_program_includes o_defines");

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

o_program_includes      :   /* Empty */
                            {
                                LOG("o_program_includes      :   /* Empty */");

                                $$ = NULL;
                            }
                        |   program_includes
                            {
                                LOG("o_program_includes      :   program_includes");

                                $$ = $1;
                            }
                        ;

program_includes        :   include_statement
                            {
                                LOG("program_includes        :   include_statement");

                                $$ = $1;
                            }
                        |   program_includes include_statement
                            {
                                LOG("program_includes        :   program_includes include_statement");

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
                                LOG("include_statement       :   TOKEN_INCLUDE TOKEN_ID TOKEN_EOS");

                                $$ = NULL;

                                char tmp[1024];

                                loader::includeNameToFilename(tmp, $2);

                                if(loader::includeFile(tmp, tmp))
                                {
                                    std::string filename = tmp;

                                    if(!data->isParsedIncludeFile(filename))
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
                            {
                                LOG("o_program_uses          :   /* Empty */");
                            }
                        |   program_uses
                            {
                                LOG("o_program_uses          :   program_uses");
                            }
                        ;

program_uses            :   use_statement
                            {
                                LOG("program_uses            :   use_statement");
                            }
                        |   program_uses use_statement
                            {
                                LOG("program_uses            :   program_uses use_statement");
                            }
                        ;

use_statement           :   TOKEN_USE TOKEN_NAME TOKEN_EOS
                            {
                                LOG("use_statement           :   TOKEN_USE TOKEN_NAME TOKEN_EOS");

                                char tmp[1024];

                                loader::useNameToFilename(tmp, $2);

                                if(loader::useFile(tmp, tmp))
                                {
                                    std::string filename = tmp;

                                    if(!data->isParsedUseFile(filename))
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

o_defines               :   /* Empty */
                            {
                                LOG("o_defines               :   /* Empty */");

                                $$ = NULL;
                            }
                        |   define
                            {
                                LOG("o_defines               :   define");

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
                        |   define defines
                            {
                                LOG("o_defines               :   define defines");

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

defines                 :   define
                            {
                                LOG("defines                 :   define");

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
                        |   defines define
                            {
                                LOG("defines                 :   defines define");

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

define                  :   s_type_definition
                            {
                                LOG("define                  :   type_definition");

                                $$ = $1;
                            }
                        |   import_statement
                            {
                                LOG("define                  :   import_statement");

                                $$ = $1;
                            }
                        |   s_constant_statement
                            {
                                LOG("define                  :   s_constant_statement");

                                $$ = $1;
                            }
                        ;

s_type_definition       :   type_definition
                            {
                                LOG("s_type_definition    :   type_definition");

                                $$ = $1;
                            }
                            /* FIXME, what to do about global scoping?? */
                        |   TOKEN_SHARED constant_statement
                            {
                                LOG("s_type_definition    :   TOKEN_SHARED type_definition");

                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

type_definition         :   TOKEN_DEF TOKEN_NAME TOKEN_EOS type_members TOKEN_END TOKEN_EOS
                            {
                                LOG("type_definition         :   TOKEN_DEF TOKEN_NAME TOKEN_EOS type_members TOKEN_END TOKEN_EOS");

                                tree::UDT *udt = new tree::UDT(std::string($2), $4);
                                udt->setLocation(@1);

                                $$ = new tree::TypeDefinition(std::string($2), udt);
                                $$->setLocation(@1);
                            }
                        ;

type_members            :   type_member
                            {
                                LOG("type_members            :   type_member");

                                $$ = new tree::Members();
                                $$->push_back($1);
                            }
                        |   type_members type_member
                            {
                                LOG("type_members            :   type_members type_member");

                                $$ = $1;
                                $$->push_back($2);
                            }
                        ;

type_member             :   TOKEN_ID TOKEN_CAST type TOKEN_EOS
                            {
                                LOG("type_member             :   TOKEN_ID TOKEN_CAST type TOKEN_EOS");

                                $$ = new tree::Member($3, std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

import_statement        :   TOKEN_IMPORT prototype TOKEN_EOS
                            {
                                LOG("import_statement        :   TOKEN_IMPORT prototype TOKEN_EOS");

                                if($2->getType() == NULL)
                                {
                                    tree::Void *type = new tree::Void();

                                    type->setLocation(@2);
                                    $2->setType(type);
                                }

                                $$ = new tree::Import($2);
                                $$->setLocation(@1);

                                $2->setTarget($$);
                            }
                        ;

s_constant_statement    :   constant_statement
                            {
                                LOG("s_constant_statement    :   constant_statement");

                                $$ = $1;
                            }
                            /* FIXME, what to do about global scoping?? */
                        |   TOKEN_SHARED constant_statement
                            {
                                LOG("s_constant_statement    :   TOKEN_SHARED constant_statement");

                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

o_declarations          :   /* Empty */
                            {
                                LOG("o_declarations          :   /* Empty */");

                                $$ = NULL;
                            }
                        |   declaration
                            {
                                LOG("o_declarations          :   declaration");

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
                        |   declaration declarations
                            {
                                LOG("o_declarations          :   declaration declarations");

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

declarations            :   declaration
                            {
                                LOG("declarations            :   declaration");

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
                        |   declarations declaration
                            {
                                LOG("declarations            :   declarations declaration");

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

declaration             :   s_variable_statement
                            {
                                LOG("declaration             :   s_variable_statement");

                                $$ = $1;
                            }
                        |   define
                            {
                                LOG("declaration             :   define");

                                $$ = $1;
                            }
                        ;

s_variable_statement    :   variable_statement
                            {
                                LOG("s_variable_statement    :   variable_statement");

                                $$ = $1;
                            }
                        |   TOKEN_GLOBAL variable TOKEN_EOS
                            {
                                LOG("s_variable_statement    :   TOKEN_GLOBAL variable TOKEN_EOS");

                                tree::Execute *execute = new tree::Execute($2);
                                execute->setLocation(@2);

                                $$ = new tree::GlobalScoping(execute);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_SHARED variable_statement
                            {
                                LOG("s_variable_statement    :   TOKEN_SHARED variable_statement");

                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

o_callables             :   /* Empty */
                            {
                                LOG("o_callables             :   /* Empty */");

                                $$ = NULL;
                            }
                        |   callable
                            {
                                LOG("o_callables             :   callable");

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
                        |   callable callables
                            {
                                LOG("o_callables             :   callable callables");

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

callables               :   callable
                            {
                                LOG("callables               :   callable");

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
                        |   callables callable
                            {
                                LOG("callables               :   callables callable");

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

callable                :   s_function
                            {
                                LOG("callable                :   s_function");

                                $$ = $1;
                            }
                        |   method
                            {
                                LOG("callable                :   method");

                                $$ = $1;
                            }
                        ;

s_function              :   function
                            {
                                LOG("s_function              :   function");

                                $$ = $1;
                            }
                            /* FIXME, what to do about global scoping?? */
                        |   TOKEN_SHARED function
                            {
                                LOG("s_function              :   TOKEN_SHARED function");

                                $$ = new tree::SharedScoping($2);
                                $$->setLocation(@1);
                            }
                        ;

function                :   TOKEN_DEF prototype TOKEN_EOS o_statements TOKEN_END TOKEN_EOS /* FIXME, support states: function_prototype function_state TOKEN_EOS o_statements TOKEN_END TOKEN_EOS */
                            {
                                LOG("function                :   TOKEN_DEF prototype TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                $$ = new tree::Function($2, $4);
                                $$->setLocation(@1);

                                $2->setTarget($$);
                            }
                        ;

method                  :   TOKEN_DEF type TOKEN_DIRECT_ACCESS prototype TOKEN_EOS o_statements TOKEN_END TOKEN_EOS /* FIXME, support states: function_prototype function_state TOKEN_EOS o_statements TOKEN_END TOKEN_EOS */
                            {
                                LOG("method                  :   TOKEN_DEF type TOKEN_DIRECT_ACCESS prototype TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                $$ = new tree::Method($2, $4, $6);
                                $$->setLocation(@1);

                                $4->setTarget($$);
                            }
                        ;

prototype               :   TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                            {
                                LOG("prototype               :   TOKEN_DEF TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE");

                                $$ = new tree::FunctionPrototype(NULL, std::string($1), $3);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE TOKEN_CAST type
                            {
                                LOG("prototype               :   TOKEN_DEF TOKEN_ID TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE TOKEN_CAST type");

                                $$ = new tree::FunctionPrototype($6, std::string($1), $3);
                                $$->setLocation(@1);
                            }
                        ;

o_arguments             :   /* Empty */
                            {
                                LOG("o_arguments             :   /* Empty */");

                                $$ = NULL;
                            }
                        |   arguments
                            {
                                LOG("o_arguments             :   arguments");

                                $$ = $1;
                            }
                        ;

arguments               :   argument
                            {
                                LOG("arguments               :   argument");

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
                                LOG("arguments               :   arguments TOKEN_COMMA argument");

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
                                LOG("argument                :   TOKEN_ID");

                                $$ = new tree::Variable(NULL, std::string($1));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_ID TOKEN_CAST type
                            {
                                LOG("argument                :   TOKEN_ID TOKEN_CAST type");

                                $$ = new tree::Variable($3, std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

/*function_state          :   *//* No state *//*
                            {
                                $$ = NULL;
                            }
                        |   TOKEN_LT state TOKEN_GT
                            {
                                $$ = $2;
                            }
                        ;*/

o_statements            :   /* Empty */
                            {
                                LOG("o_statements            :   /* Empty */");

                                $$ = NULL;
                            }
                        |   statements
                            {
                                LOG("o_statements            :   statements");

                                $$ = $1;
                            }
                        ;

statements              :   statement
                            {
                                LOG("statements              :   statement");

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
                                LOG("statements              :   statements statement");

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

statement               :   constant_statement
                            {
                                LOG("statement               :   constant_statement");

                                $$ = $1;
                            }
                        |   variable_statement
                            {
                                LOG("statement               :   variable_statement");

                                $$ = $1;
                            }
                        |   conditional_statement
                            {
                                LOG("statement               :   conditional_statement");

                                $$ = $1;
                            }
                        |   loop_statement
                            {
                                LOG("statement               :   loop_statement");

                                $$ = $1;
                            }
                        |   single_statement TOKEN_EOS
                            {
                                LOG("statement               :   single_statement TOKEN_EOS");

                                $$ = $1;
                            }
                        |   block_statement
                            {
                                LOG("statement               :   block_statement");

                                $$ = $1;
                            }
                        ;

constant_statement      :   constant_assignment TOKEN_EOS
                            {
                                LOG("constant_statement      :   constant_assignment TOKEN_EOS");

                                $$ = new tree::Execute($1);
                                $$->setLocation(@1);
                            }
                        ;

constant_assignment     :   constant TOKEN_EQUALS expression
                            {
                                LOG("constant_assignment     :   constant TOKEN_EQUALS expression");

                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

constant                :   TOKEN_CONST TOKEN_NAME
                            {
                                LOG("constant                :   TOKEN_CONST TOKEN_NAME");

                                $$ = new tree::Constant(NULL, std::string($2));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_CONST TOKEN_NAME TOKEN_CAST type
                            {
                                LOG("constant                :   TOKEN_CONST TOKEN_NAME TOKEN_CAST type");

                                $$ = new tree::Constant($4, std::string($2));
                                $$->setLocation(@1);
                            }
                        ;

variable_statement      :   variable_assignment TOKEN_EOS
                            {
                                LOG("variable_statement      :   variable_assignment TOKEN_EOS");

                                $$ = new tree::Execute($1);
                                $$->setLocation(@1);
                            }
                        ;

variable_assignment     :   variable
                            {
                                LOG("variable_assignment     :   variable");

                                $$ = $1;
                            }
                        |   variable TOKEN_EQUALS expression
                            {
                                LOG("variable_assignment     :   variable TOKEN_EQUALS expression");

                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

variable                :   TOKEN_DEF TOKEN_ID
                            {
                                LOG("variable                :   TOKEN_DEF TOKEN_ID");

                                $$ = new tree::Variable(NULL, std::string($2));
                                $$->setLocation(@1);
                            }
                        |   TOKEN_DEF TOKEN_ID TOKEN_CAST type
                            {
                                LOG("variable                :   TOKEN_DEF TOKEN_ID TOKEN_CAST type");

                                $$ = new tree::Variable($4, std::string($2));
                                $$->setLocation(@1);
                            }
                        ;

conditional_statement   :   single_statement TOKEN_IF expression TOKEN_EOS
                            {
                                LOG("conditional_statement   :   single_statement TOKEN_IF expression TOKEN_EOS");

                                $$ = new tree::If($3, $1);
                                $$->setLocation(@2);
                            }
                        |   single_statement TOKEN_IF expression TOKEN_ELSE single_statement TOKEN_EOS
                            {
                                LOG("conditional_statement   :   single_statement TOKEN_IF expression TOKEN_ELSE single_statement TOKEN_EOS");

                                $$ = new tree::If($3, $1, $5);
                                $$->setLocation(@2);
                            }
                        ;



loop_statement          :   single_statement TOKEN_FOR argument TOKEN_IN expression TOKEN_EOS
                            {
                                LOG("loop_statement          :   single_statement TOKEN_FOR argument TOKEN_IN expression TOKEN_EOS");

                                tree::Statements *forStatement = new tree::Statements();
                                forStatement->push_back($1);

                                $$ = new tree::For(forStatement, $3, $5);
                                $$->setLocation(@2);
                            }
                        |   single_statement TOKEN_WHILE expression TOKEN_EOS
                            {
                                LOG("loop_statement          :   single_statement TOKEN_WHILE expression TOKEN_EOS");

                                $$ = new tree::While($3, $1);
                                $$->setLocation(@2);
                            }
                        ;

single_statement        :   execute_statement
                            {
                                LOG("single_statement        :   execute_statement");

                                $$ = $1;
                            }
                        |   return_statement
                            {
                                LOG("single_statement        :   return_statement");

                                $$ = $1;
                            }

                        /*|   state_statement
                            {
                                LOG("single_statement        :   state_statement");

                                $$ = $1;
                            }*/
                        ;

execute_statement       :   assign_or_function
                            {
                                LOG("execute_statement       :   assign_or_function");

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

assign_or_function      :   assignment
                            {
                                LOG("assign_or_function      :   assignment");

                                $$ = $1;
                            }
                        |   access_call_expression
                            {
                                LOG("assign_or_function      :   access_call_expression");

                                $$ = $1;
                            }

                        |   error                                                                       /* Special case!! Checking here will catch a lot of cases, should produce better error reports */
                            {
                                LOG("assign_or_function      :   error");

                                $$ = NULL; // FIXME
                            }
                        ;

assignment              :   access_assignee TOKEN_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_EQUALS expression");

                                $$ = new tree::Assign($1, $3);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_OR_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_OR_EQUALS expression");

                                tree::Or *orExpression = new tree::Or($1, $3);
                                orExpression->setLocation(@2);

                                $$ = new tree::Assign($1, orExpression);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_XOR_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_XOR_EQUALS expression");

                                tree::Xor *xorExpression = new tree::Xor($1, $3);
                                xorExpression->setLocation(@2);

                                $$ = new tree::Assign($1, xorExpression);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_AND_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_AND_EQUALS expression");

                                tree::And *andExpression = new tree::And($1, $3);
                                andExpression->setLocation(@2);

                                $$ = new tree::Assign($1, andExpression);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_ADD_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_ADD_EQUALS expression");

                                tree::Add *add = new tree::Add($1, $3);
                                add->setLocation(@2);

                                $$ = new tree::Assign($1, add);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_SUBTRACT_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_SUBTRACT_EQUALS expression");

                                tree::Subtract *subtract = new tree::Subtract($1, $3);
                                subtract->setLocation(@2);

                                $$ = new tree::Assign($1, subtract);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_MULTIPLY_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_MULTIPLY_EQUALS expression");

                                tree::Multiply *multiply = new tree::Multiply($1, $3);
                                multiply->setLocation(@2);

                                $$ = new tree::Assign($1, multiply);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_DIVIDE_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_DIVIDE_EQUALS expression");

                                tree::Divide *divide = new tree::Divide($1, $3);
                                divide->setLocation(@2);

                                $$ = new tree::Assign($1, divide);
                                $$->setLocation(@2);
                            }
                        |   access_assignee TOKEN_MODULUS_EQUALS expression
                            {
                                LOG("assignment              :   access_assignee TOKEN_MODULUS_EQUALS expression");

                                tree::Modulus *modulus = new tree::Modulus($1, $3);
                                modulus->setLocation(@2);

                                $$ = new tree::Assign($1, modulus);
                                $$->setLocation(@2);
                            }
                        ;

access_assignee         :   assignee
                            {
                                LOG("access_assignee         :   assignee");

                                $$ = $1;
                            }
                        |   TOKEN_DIRECT_ACCESS assignee
                            {
                                LOG("access_assignee         :   TOKEN_DIRECT_ACCESS assignee");

                                tree::This *th1s = new tree::This();
                                th1s->setLocation(@1);

                                $$ = new tree::DirectAccess(th1s, $2);
                                $$->setLocation(@1);
                            }
                        |   access_assignee TOKEN_DIRECT_ACCESS assignee
                            {
                                LOG("access_assignee         :   access_assignee TOKEN_DIRECT_ACCESS assignee");

                                $$ = new tree::DirectAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

assignee                :   identifier
                            {
                                LOG("assignee                :   identifier");

                                $<id>$ = $1;
                            }
                        |   assignee TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                            {
                                LOG("assignee                :   assignee TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE");

                                $$ = new tree::ArrayAccess($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

expression              :   l_or_expression
                            {
                                LOG("expression              :   l_or_expression");

                                $$ = $1;
                            }
                        ;

l_or_expression         :   l_and_expression
                            {
                                LOG("l_or_expression         :   l_and_expression");

                                $$ = $1;
                            }
                        |   l_or_expression TOKEN_LOGICAL_OR l_and_expression
                            {
                                LOG("l_or_expression         :   l_or_expression TOKEN_LOGICAL_OR l_and_expression");

                                $$ = new tree::LogicalOr($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

l_and_expression        :   or_expression
                            {
                                LOG("l_and_expression        :   or_expression");

                                $$ = $1;
                            }
                        |   l_and_expression TOKEN_LOGICAL_AND or_expression
                            {
                                LOG("l_and_expression        :   l_and_expression TOKEN_LOGICAL_AND or_expression");

                                $$ = new tree::LogicalAnd($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

or_expression           :   xor_expression
                            {
                                LOG("or_expression           :   xor_expression");

                                $$ = $1;
                            }
                        |   or_expression TOKEN_OR xor_expression
                            {
                                LOG("or_expression           :   or_expression TOKEN_OR xor_expression");

                                $$ = new tree::Or($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

xor_expression          :   and_expression
                            {
                                LOG("xor_expression          :   and_expression");

                                $$ = $1;
                            }
                        |   xor_expression TOKEN_XOR and_expression
                            {
                                LOG("xor_expression          :   xor_expression TOKEN_XOR and_expression");

                                $$ = new tree::Xor($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

and_expression          :   eq_expression
                            {
                                LOG("and_expression          :   eq_expression");

                                $$ = $1;
                            }
                        |   and_expression TOKEN_AND eq_expression
                            {
                                LOG("and_expression          :   and_expression TOKEN_AND eq_expression");

                                $$ = new tree::And($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

eq_expression           :   rel_expression
                            {
                                LOG("eq_expression           :   rel_expression");

                                $$ = $1;
                            }
                        |   eq_expression TOKEN_EQ rel_expression
                            {
                                LOG("eq_expression           :   eq_expression TOKEN_EQ rel_expression");

                                $$ = new tree::Equal($1, $3);
                                $$->setLocation(@2);
                            }
                        |   eq_expression TOKEN_NE rel_expression
                            {
                                LOG("eq_expression           :   eq_expression TOKEN_NE rel_expression");

                                $$ = new tree::Unequal($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

rel_expression          :   add_expression
                            {
                                LOG("rel_expression          :   add_expression");

                                $$ = $1;
                            }
                        |   rel_expression TOKEN_LT add_expression
                            {
                                LOG("rel_expression          :   rel_expression TOKEN_LT add_expression");

                                $$ = new tree::LessThan($1, $3);
                                $$->setLocation(@2);
                            }
                        |   rel_expression TOKEN_LE add_expression
                            {
                                LOG("rel_expression          :   rel_expression TOKEN_LE add_expression");

                                $$ = new tree::LessEqual($1, $3);
                                $$->setLocation(@2);
                            }
                        |   rel_expression TOKEN_GT add_expression
                            {
                                LOG("rel_expression          :   rel_expression TOKEN_GT add_expression");

                                $$ = new tree::GreaterThan($1, $3);
                                $$->setLocation(@2);
                            }
                        |   rel_expression TOKEN_GE add_expression
                            {
                                LOG("rel_expression          :   rel_expression TOKEN_GE add_expression");

                                $$ = new tree::GreaterEqual($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

add_expression          :   mult_expression
                            {
                                LOG("add_expression          :   mult_expression");

                                $$ = $1;
                            }
                        |   add_expression TOKEN_ADD mult_expression
                            {
                                LOG("add_expression          :   add_expression TOKEN_ADD mult_expression");

                                $$ = new tree::Add($1, $3);
                                $$->setLocation(@2);
                            }
                        |   add_expression TOKEN_SUBTRACT mult_expression
                            {
                                LOG("add_expression          :   add_expression TOKEN_SUBTRACT mult_expression");

                                $$ = new tree::Subtract($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

mult_expression         :   cast_expression
                            {
                                LOG("mult_expression         :   cast_expression");

                                $$ = $1;
                            }
                        |   mult_expression TOKEN_MULTIPLY cast_expression
                            {
                                LOG("mult_expression         :   mult_expression TOKEN_MULTIPLY cast_expression");

                                $$ = new tree::Multiply($1, $3);
                                $$->setLocation(@2);
                            }
                        |   mult_expression TOKEN_DIVIDE cast_expression
                            {
                                LOG("mult_expression         :   mult_expression TOKEN_DIVIDE cast_expression");

                                $$ = new tree::Divide($1, $3);
                                $$->setLocation(@2);
                            }
                        |   mult_expression TOKEN_MODULUS cast_expression
                            {
                                LOG("mult_expression         :   mult_expression TOKEN_MODULUS cast_expression");

                                $$ = new tree::Modulus($1, $3);
                                $$->setLocation(@2);
                            }
                        ;

cast_expression         :   unary_expression
                            {
                                LOG("cast_expression         :   unary_expression");

                                $$ = $1;
                            }
                        |   expression_atom TOKEN_CAST type
                            {
                                LOG("cast_expression         :   expression_atom TOKEN_CAST type");

                                $$ = new tree::Cast($3, $1);
                                $$->setLocation(@2);
                            }
                        ;

unary_expression        :   access_expression
                            {
                                LOG("unary_expression        :   access_expression");

                                $$ = $1;
                            }
                        |   TOKEN_SUBTRACT access_expression
                            {
                                LOG("unary_expression        :   TOKEN_SUBTRACT access_expression");

                                $$ = new tree::Minus($2);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_LOGICAL_NOT access_expression
                            {
                                LOG("unary_expression        :   TOKEN_LOGICAL_NOT access_expression");

                                $$ = new tree::LogicalNot($2);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_NOT access_expression
                            {
                                LOG("unary_expression        :   TOKEN_NOT access_expression");

                                $$ = new tree::Not($2);
                                $$->setLocation(@1);
                            }
                        ;

access_expression       :   postfix_expression
                            {
                                LOG("access_expression       :   postfix_expression");

                                $$ = $1;
                            }
                        |   TOKEN_DIRECT_ACCESS access_expression
                            {
                                LOG("access_expression       :   TOKEN_DIRECT_ACCESS access_expression");

                                tree::This *th1s = new tree::This();
                                th1s->setLocation(@1);

                                $$ = new tree::DirectAccess(th1s, $2);
                                $$->setLocation(@1);
                            }
                        |   access_expression TOKEN_DIRECT_ACCESS postfix_expression
                            {
                                LOG("access_expression       :   postfix_expression TOKEN_DIRECT_ACCESS postfix_expression");

                                $$ = new tree::DirectAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        /*|   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression
                            {
                                LOG("access_expression       :   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression");

                                $$ = new tree::MessageAccess($1, $3);
                                $$->setLocation(@1);
                            }*/
                        ;

postfix_expression      :   expression_atom
                            {
                                LOG("postfix_expression      :   expression_atom");

                                $$ = $1;
                            }
                        |   array_expression
                            {
                                LOG("postfix_expression      :   array_expression");

                                $$ = $1;
                            }
                        |   call_expression
                            {
                                LOG("postfix_expression      :   call_expression");

                                $$ = $1;
                            }
                        ;

array_expression        :   postfix_expression TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                            {
                                LOG("array_expression        :   postfix_expression TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE");

                                $$ = new tree::ArrayAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        ;

access_call_expression  :   call_expression
                            {
                                $$ = $1;
                            }
                        |   TOKEN_DIRECT_ACCESS call_expression
                            {
                                LOG("access_call_expression       :   TOKEN_DIRECT_ACCESS call_expression");

                                tree::This *th1s = new tree::This();
                                th1s->setLocation(@1);

                                $$ = new tree::DirectAccess(th1s, $2);
                                $$->setLocation(@1);
                            }
                        |   postfix_expression TOKEN_DIRECT_ACCESS call_expression
                            {
                                LOG("access_call_expression       :   postfix_expression TOKEN_DIRECT_ACCESS call_expression");

                                $$ = new tree::DirectAccess($1, $3);
                                $$->setLocation(@1);
                            }
                        /*|   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression
                            {
                                LOG("access_expression       :   postfix_expression TOKEN_MESSAGE_ACCESS postfix_expression");

                                $$ = new tree::MessageAccess($1, $3);
                                $$->setLocation(@1);
                            }*/
                        ;

call_expression         :   identifier TOKEN_PARENTHESIS_OPEN o_argument_expressions TOKEN_PARENTHESIS_CLOSE
                            {
                                LOG("call_expression         :   identifier TOKEN_PARENTHESIS_OPEN o_argument_expressions TOKEN_PARENTHESIS_CLOSE");

                                // This C style cast is nasty but done for good reason.
                                $$ = new tree::FunctionCall((tree::FunctionPrototype *)$1, $3);
                                $$->setLocation(@1);
                            }
                        ;

o_argument_expressions  :   /* Empty */
                            {
                                LOG("o_argument_expressions  :   /* Empty */");

                                $$ = NULL;
                            }
                        |   argument_expressions
                            {
                                LOG("o_argument_expressions  :   argument_expressions");

                                $$ = $1;
                            }
                        ;

argument_expressions    :   expression
                            {
                                LOG("argument_expressions    :   expression");

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
                                LOG("argument_expressions    :   argument_expressions TOKEN_COMMA expression");

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
                                LOG("expression_atom         :   name");

                                $<id>$ = $1;
                            }
                        |   TOKEN_TRUE
                            {
                                LOG("expression_atom         :   TOKEN_TRUE");

                                $$ = new tree::BoolLiteral(true);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_FALSE
                            {
                                LOG("expression_atom         :   TOKEN_FALSE");

                                $$ = new tree::BoolLiteral(false);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_INTEGER
                            {
                                LOG("expression_atom         :   TOKEN_INTEGER");

                                $$ = new tree::IntLiteral($1);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_FLOAT
                            {
                                LOG("expression_atom         :   TOKEN_FLOAT");

                                $$ = new tree::FloatLiteral($1);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_STRING
                            {
                                LOG("expression_atom         :   TOKEN_STRING");

                                $$ = new tree::StringLiteral(std::string($1));
                                $$->setLocation(@1);
                            }
                        |   identifier
                            {
                                LOG("expression_atom         :   identifier");

                                $<id>$ = $1;
                            }
                        |   TOKEN_PARENTHESIS_OPEN conditional_expression TOKEN_PARENTHESIS_CLOSE
                            {
                                LOG("expression_atom         :   TOKEN_PARENTHESIS_OPEN conditional_expression TOKEN_PARENTHESIS_CLOSE");

                                $$ = $2;
                            }
                        |   expression TOKEN_RANGE expression
                            {
                                LOG("expression_atom         :   expression TOKEN_RANGE expression");

                                $$ = new tree::ComputedArray($1, $3);
                            }
                        |   expression TOKEN_RANGE expression TOKEN_COMMA expression
                            {
                                LOG("expression_atom         :   expression TOKEN_RANGE expression TOKEN_RANGE expression");

                                $$ = new tree::ComputedArray($1, $3, $5);
                            }
                        ;

conditional_expression  :   expression
                            {
                                LOG("conditional_expression  :   expression");

                                $$ = $1;
                            }
                        |   expression TOKEN_IF expression
                            {
                                LOG("conditional_expression  :   expression TOKEN_IF expression");

                                $$ = new tree::IfExpression($3, $1);
                                $$->setLocation(@2);
                            }
                        |   expression TOKEN_IF expression TOKEN_ELSE expression
                            {
                                LOG("conditional_expression  :   expression TOKEN_IF expression TOKEN_ELSE expression");

                                $$ = new tree::IfExpression($3, $1, $5);
                                $$->setLocation(@2);
                            }
                        ;

type                    :   TOKEN_TYPE_BOOL
                            {
                                LOG("type                    :   TOKEN_TYPE_BOOL");

                                $$ = new tree::Bool();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_INT
                            {
                                LOG("type                    :   TOKEN_TYPE_INT");

                                $$ = new tree::Int();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_INT TOKEN_LT expression TOKEN_GT
                            {
                                LOG("type                    :   TOKEN_TYPE_INT TOKEN_LT expression TOKEN_GT");

                                $$ = new tree::Int($3);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_FLOAT
                            {
                                LOG("type                    :   TOKEN_TYPE_FLOAT");

                                $$ = new tree::Float();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_FLOAT TOKEN_LT expression TOKEN_GT
                            {
                                LOG("type                    :   TOKEN_TYPE_FLOAT TOKEN_LT expression TOKEN_GT");

                                $$ = new tree::Float($3);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_STRING
                            {
                                LOG("type                    :   TOKEN_TYPE_STRING");

                                $$ = new tree::String();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_TYPE_STRING TOKEN_LT expression TOKEN_GT
                            {
                                LOG("type                    :   TOKEN_TYPE_STRING TOKEN_LT expression TOKEN_GT");

                                $$ = new tree::String($3);
                                $$->setLocation(@1);
                            }
                        |   name
                            {
                                LOG("type                    :   name");

                                $<id>$ = $1;
                            }
                        |   TOKEN_BRACKETS_OPEN TOKEN_BRACKETS_CLOSE
                            {
                                LOG("type                    :   TOKEN_BRACKETS_OPEN TOKEN_BRACKETS_CLOSE");

                                $$ = new tree::Array(NULL);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                            {
                                LOG("type                    :   TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE");

                                $$ = new tree::Array(NULL, $2);
                                $$->setLocation(@1);
                            }
                        |   type TOKEN_BRACKETS_OPEN TOKEN_BRACKETS_CLOSE
                            {
                                LOG("type                    :   type TOKEN_BRACKETS_OPEN TOKEN_BRACKETS_CLOSE");

                                tree::Array *array = dynamic_cast<tree::Array *>($1);

                                if(array)
                                {
                                    for(tree::Array *nextArray = dynamic_cast<tree::Array *>(array->getType()); nextArray; array = nextArray, nextArray = dynamic_cast<tree::Array *>(array->getType()))
                                        ;

                                    tree::Array *childArray = new tree::Array(array->getType());
                                    childArray->setLocation(@2);

                                    array->setType(childArray);

                                    $$ = $1;
                                }
                                else
                                {
                                    $$ = new tree::Array($1);
                                    $$->setLocation(@2);
                                }
                            }
                        |   type TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE
                            {
                                LOG("type                    :   type TOKEN_BRACKETS_OPEN expression TOKEN_BRACKETS_CLOSE");

                                tree::Array *array = dynamic_cast<tree::Array *>($1);

                                if(array)
                                {
                                    for(tree::Array *nextArray = dynamic_cast<tree::Array *>(array->getType()); nextArray; array = nextArray, nextArray = dynamic_cast<tree::Array *>(array->getType()))
                                        ;

                                    tree::Array *childArray = new tree::Array(array->getType(), $3);
                                    childArray->setLocation(@2);

                                    array->setType(childArray);

                                    $$ = $1;
                                }
                                else
                                {
                                    $$ = new tree::Array($1, $3);
                                    $$->setLocation(@2);
                                }
                            }
                        ;

identifier              :   TOKEN_ID
                            {
                                LOG("identifier              :   TOKEN_ID");

                                $$ = new tree::Identifier(std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

name                    :   TOKEN_NAME
                            {
                                LOG("name                    :   TOKEN_NAME");

                                $$ = new tree::Identifier(std::string($1));
                                $$->setLocation(@1);
                            }
                        ;

block_statement         :   if_statement
                            {
                                LOG("block_statement         :   if_statement");

                                $$ = $1;
                            }
                        |   for_statement
                            {
                                LOG("block_statement         :   for_statement");

                                $$ = $1;
                            }
                        |   while_statement
                            {
                                LOG("block_statement         :   while_statement");

                                $$ = $1;
                            }
                        ;

if_statement            :   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS
                            {
                                LOG("if_statement            :   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                tree::AnonymousScope *trueStatements = new tree::AnonymousScope($4);
                                trueStatements->setLocation(@4);

                                $$ = new tree::If($2, trueStatements);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_ELSE TOKEN_EOS o_statements TOKEN_END TOKEN_EOS
                            {
                                LOG("if_statement            :   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_ELSE TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                tree::AnonymousScope *trueStatements = new tree::AnonymousScope($4);
                                trueStatements->setLocation(@4);

                                tree::AnonymousScope *falseStatements = new tree::AnonymousScope($7);
                                falseStatements->setLocation(@7);

                                $$ = new tree::If($2, trueStatements, falseStatements);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_ELSE if_statement
                            {
                                LOG("if_statement            :   TOKEN_IF expression TOKEN_EOS o_statements TOKEN_ELSE if_statement");

                                tree::AnonymousScope *trueStatements = new tree::AnonymousScope($4);
                                trueStatements->setLocation(@4);

                                $$ = new tree::If($2, trueStatements, $6);
                                $$->setLocation(@1);
                            }
                        ;

for_statement           :   TOKEN_FOR argument TOKEN_IN expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS
                            {
                                LOG("for_statement           :   TOKEN_FOR argument TOKEN_IN expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                $$ = new tree::For($6, $2, $4);
                                $$->setLocation(@1);
                            }
                        ;

while_statement         :   TOKEN_WHILE expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS
                            {
                                LOG("while_statement         :   TOKEN_WHILE expression TOKEN_EOS o_statements TOKEN_END TOKEN_EOS");

                                tree::AnonymousScope *loopStatements = new tree::AnonymousScope($4);
                                loopStatements->setLocation(@4);

                                $$ = new tree::While($2, loopStatements);
                                $$->setLocation(@1);
                            }
                        ;

return_statement        :   TOKEN_RETURN
                            {
                                LOG("return_statement        :   TOKEN_RETURN");

                                $$ = new tree::Return(NULL);
                                $$->setLocation(@1);
                            }
                        |   TOKEN_RETURN expression
                            {
                                LOG("return_statement        :   TOKEN_RETURN expression");

                                $$ = new tree::Return($2);
                                $$->setLocation(@1);
                            }
                        ;

/*state_statement         :   TOKEN_STATE state
                            {
                                LOG("state_statement         :   TOKEN_STATE state");

                                $$ = new tree::SetState($2);
                                $$->setLocation(@1);
                            }
                        ;*/

/*state                   :   TOKEN_RESET
                            {
                                LOG("state                   :   TOKEN_RESET");

                                $$ = new tree::DefaultState();
                                $$->setLocation(@1);
                            }
                        |   TOKEN_NAME
                            {
                                LOG("state                   :   TOKEN_NAME");

                                $$ = new tree::NamedState($1);
                                $$->setLocation(@1);
                            }
                        ;*/

%%

void yyerror(YYLTYPE *locp, void *lexer, const char *error)
{
    error::enqueue(*locp, error);
}

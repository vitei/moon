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
    #include "__parser.h"
    #include "__lexer.h"

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
    parser::StatementList list;

    tree::FunctionPrototype *prototype;
    tree::Statement *statement;
    tree::Expression *expression;
    tree::Type *type;
    tree::Identifier *id;
    tree::State *state;

    /* The lexer returns these... */
    int integer;
    float real;
    char string[1024]; /* FIXME: We should make this bigger... */
}

/* Start tokens */
%token START_PROGRAM
%token START_INCLUDE

/* Language structure */
%token TOKEN_EOS
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

/* Built-In Types */
%token TOKEN_TYPE_INT
%token TOKEN_TYPE_FLOAT
%token TOKEN_TYPE_STRING

/* Identifiers */
%token<string> TOKEN_NAME
%token<string> TOKEN_ID

/* Return types */
%type<list> program
%type<list> include
%type<list> o_program_includes
%type<list> program_includes
%type<list> include_statements
%type<list> o_program_uses
%type<list> program_uses
%type<list> use_statement 
%type<list> o_program_cvrs
%type<list> program_cvrs
%type<statement> program_cvr
%type<statement> constant_statement
%type<expression> constant_assignment
%type<expression> constant
%type<statement> scoped_var_statement
%type<statement> scoped_ref_statement
%type<list> o_program_functions
%type<list> program_functions
%type<statement> program_function
%type<prototype> function_prototype
%type<list> o_arguments
%type<list> arguments
%type<statement> argument
%type<state> function_state
%type<list> statement_block
%type<list> statements
%type<statement> statement
%type<statement> variable_statement
%type<expression> variable_assignment
%type<expression> variable
%type<statement> reference_statement
%type<expression> reference_assignment
%type<expression> reference
%type<statement> a_or_e_statement
%type<expression> assign_or_expression
%type<statement> expression_statement
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
%type<list> argument_expressions
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

start               :   START_PROGRAM program
                        {
                            data->list.head = $2.head;
                            data->list.tail = $2.tail;
                        }
                    |   START_INCLUDE include
                        {
                            data->list.head = $2.head;
                            data->list.tail = $2.tail;
                        }
                    ;

program             :   o_program_includes o_program_uses o_program_cvrs o_program_functions
                        {
                            $$.head = NULL;
                            $$.tail = NULL;

                            // Check there is actually something in this scope...
                            if($1.head || $3.head || $4.head)
                            {
                                tree::Statement *lastStatement;

                                if($1.head)
                                {
                                    $$.head = new tree::Scope($1.head);
                                    $$.tail = $$.head;

                                    lastStatement = $1.tail;
                                }

                                if($3.head)
                                {
                                    if(!$$.head)
                                    {
                                        $$.head = new tree::Scope($3.head);
                                        $$.tail = $$.head;
                                    }
                                    else
                                    {
                                        lastStatement->setNext($3.head);
                                    }

                                    lastStatement = $3.tail;
                                }

                                if($4.head)
                                {
                                    if(!$$.head)
                                    {
                                        $$.head = new tree::Scope($4.head);
                                        $$.tail = $$.head;
                                    }
                                    else
                                    {
                                        lastStatement->setNext($4.head);
                                    }

                                    lastStatement = $4.tail;
                                }
                            }

                            // Uses are special because they should be on the same level of scope as this
                            if($2.head)
                            {
                                if($$.head)
                                {
                                    $$.head->setNext($2.head);
                                    $$.tail = $2.tail;
                                }
                                else
                                {
                                    $$ = $2;
                                }
                            }
                        }
                    ;

include             :   o_program_includes o_program_cvrs /* FIXME */
                        {
                            parser::StatementList list;

                            if($1.head)
                            {
                                list = $1;
                            }

                            if($2.head)
                            {
                                if(list.tail)
                                {
                                    list.tail->setNext($2.head);
                                    list.tail = $2.tail;
                                }
                                else
                                {
                                    list.head = $2.head;
                                }

                                list.tail = $2.tail;
                            }

                            $$ = list;
                        }
                    ;

o_program_includes  :   /* Empty */
                        {
                            $$.head = NULL;
                        }
                    |   program_includes
                        {
                            $$ = $1;
                        }
                    ;

program_includes    :   include_statements
                        {
                            $$ = $1;
                        }
                    |   program_includes include_statements
                        {
                            $$ = $1;

                            if($2.head)
                            {
                                if($1.head)
                                {
                                    $$ = $1;
                                    $$.tail->setNext($2.head);
                                    $$.tail = $2.tail;
                                }
                                else
                                {
                                    $$ = $2;
                                }
                            }
                        }
                    ;

include_statements  :   TOKEN_INCLUDE TOKEN_ID TOKEN_EOS
                        {
                            char tmp[1024];

                            $$.head = NULL;
                            $$.tail = NULL;

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

                                    data->list.head = NULL;
                                    data->list.tail = NULL;

                                    yyparse(data);

                                    loader::popCWD();

                                    yylex_destroy(data->lexer);
                                    fclose(input);

                                    data->lexer = currentLexer;

                                    $$.head = data->list.head;
                                    $$.tail = data->list.tail;
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

o_program_uses      :   /* Empty */
                        {
                            $$.head = NULL;
                        }
                    |   program_uses
                        {
                            $$ = $1;
                        }
                    ;

program_uses        :   use_statement
                        {
                            $$ = $1;
                        }
                    |   program_uses use_statement
                        {
                            $$ = $1;

                            if($2.head)
                            {
                                $$.tail->setNext($2.head);
                                $$.tail = $2.tail;
                            }
                        }
                    ;

use_statement       :   TOKEN_USE TOKEN_NAME TOKEN_EOS
                        {
                            char tmp[1024];

                            $$.head = NULL;
                            $$.tail = NULL;

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

                                    data->list.head = NULL;
                                    data->list.tail = NULL;

                                    yyparse(data);

                                    loader::popCWD();

                                    yylex_destroy(data->lexer);
                                    fclose(input);

                                    data->lexer = currentLexer;

                                    $$ = data->list;
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
                            $$.head = NULL;
                        }
                    |   program_cvrs
                        {
                            $$ = $1;
                        }
                    ;

program_cvrs        :   program_cvr                                                                 /* CVRs = constants + variables + references */
                        {
                            $$.head = $1;
                            $$.tail = $1;
                        }
                    |   program_cvrs program_cvr
                        {
                            $$ = $1;
                            $$.tail->setNext($2);
                            $$.tail = $2;
                        }
                    ;

program_cvr         :   constant_statement
                        {
                            $$ = $1;
                        }
                    |   scoped_var_statement
                        {
                            $$ = $1;
                        }
                    |   scoped_ref_statement
                        {
                            $$ = $1;
                        }
                    ;

constant_statement  :   constant_assignment TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
                        }
                    ;

constant_assignment :   constant TOKEN_EQUALS expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ASSIGN, $1, $3);
                        }
                    ;

constant            :   TOKEN_CONST name
                        {
                            tree::Type *type = new tree::Type(tree::Type::DATA_INT);
                            $$ = new tree::Constant(type, $2);
                        }
                    |   TOKEN_CONST type TOKEN_CAST name
                        {
                            $$ = new tree::Constant($2, $4);
                        }
                    ;

scoped_var_statement:   variable_assignment TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
                        }
                    |   TOKEN_GLOBAL variable_assignment TOKEN_EOS
                        {
                            tree::GlobalNodeScope *scope = new tree::GlobalNodeScope($2);
                            $$ = new tree::ExpressionStatement(scope);
                        }
                    |   TOKEN_SHARED variable_assignment TOKEN_EOS
                        {
                            tree::SharedNodeScope *scope = new tree::SharedNodeScope($2);
                            $$ = new tree::ExpressionStatement(scope);
                        }
                    ;

scoped_ref_statement:   reference_assignment TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
                        }
                    |   TOKEN_GLOBAL reference_assignment TOKEN_EOS
                        {
                            tree::GlobalNodeScope *scope = new tree::GlobalNodeScope($2);
                            $$ = new tree::ExpressionStatement(scope);
                        }
                    |   TOKEN_SHARED reference_assignment TOKEN_EOS
                        {
                            tree::SharedNodeScope *scope = new tree::SharedNodeScope($2);
                            $$ = new tree::ExpressionStatement(scope);
                        }
                    ;

o_program_functions :   /* Empty */
                        {
                            $$.head = NULL;
                        }
                    |   program_functions
                        {
                            $$ = $1;
                        }
                    ;

program_functions   :   program_function
                        {
                            $$.head = $1;
                            $$.tail = $1;
                        }
                    |   program_functions program_function
                        {
                            $$ = $1;
                            $$.tail->setNext($2);
                            $$.tail = $2;
                        }
                    ;

program_function    :   function_prototype function_state statement_block /* FIXME, support states */
                        {
                            $$ = new tree::Function($1, $3.head);
                        }
                    ;

function_prototype  :   TOKEN_FUNCTION identifier TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                        {
                            tree::Type *type = new tree::Type(tree::Type::DATA_INT);
                            $$ = new tree::FunctionPrototype(type, $2, $4.head);
                        }
                    |   TOKEN_FUNCTION type TOKEN_CAST identifier TOKEN_PARENTHESIS_OPEN o_arguments TOKEN_PARENTHESIS_CLOSE
                        {
                            $$ = new tree::FunctionPrototype($2, $4, $6.head);
                        }
                    ;

o_arguments         :   /* Empty */
                        {
                            $$.head = NULL;
                        }
                    |   arguments
                        {
                            $$ = $1;
                        }
                    ;

arguments           :   argument
                        {
                            $$.head = $1;
                            $$.tail = $1;
                        }
                    |   arguments TOKEN_COMMA argument
                        {
                            $$ = $1;
                            $$.tail->setNext($3);
                            $$.tail = $3;
                        }
                    ;

argument            :   identifier
                        {
                            tree::Type *type = new tree::Type(tree::Type::DATA_INT);
                            tree::Expression *expression = new tree::Variable(type, $1);
                            $$ = new tree::ExpressionStatement(expression);
                        }
                    |   type TOKEN_CAST identifier
                        {
                            tree::Expression *expression = new tree::Variable($1, $3);
                            $$ = new tree::ExpressionStatement(expression);
                        }
                    |   variable
                        {
                            tree::Expression *expression = $1;
                            $$ = new tree::ExpressionStatement(expression);
                        }
                    |   reference
                        {
                            tree::Expression *expression = $1;
                            $$ = new tree::ExpressionStatement(expression);
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

statement_block     :   TOKEN_BRACE_OPEN TOKEN_BRACE_CLOSE                                          /* Empty... */
                        {
                            $$.head = NULL;
                        }
                    |   TOKEN_BRACE_OPEN statements TOKEN_BRACE_CLOSE
                        {
                            $$ = $2;
                        }
                    ;

statements          :   statement
                        {
                            $$.head = $1;
                            $$.tail = $1;
                        }
                    |   statements statement
                        {
                            $$ = $1;
                            $$.tail->setNext($2);
                            $$.tail = $2;
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
                    /*|   statement_block       FIXME, do we want to support this??
                        {
                            $$ = $1;
                        }*/
                    ;

variable_statement  :   variable_assignment TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
                        }
                    ;

variable_assignment :   variable
                        {
                            $$ = $1;
                        }
                    |   variable TOKEN_EQUALS expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ASSIGN, $1, $3);
                        }
                    ;

variable            :   TOKEN_VAR identifier
                        {
                            tree::Type *type = new tree::Type(tree::Type::DATA_INT);
                            $$ = new tree::Variable(type, $2);
                        }
                    |   TOKEN_VAR type TOKEN_CAST identifier
                        {
                            $$ = new tree::Variable($2, $4);
                        }
                    ;

reference_statement :   reference_assignment TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
                        }
                    ;

reference_assignment:   reference
                        {
                            $$ = $1;
                        }
                    |   reference TOKEN_EQUALS expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ASSIGN, $1, $3);
                        }
                    ;

reference           :   TOKEN_REF identifier
                        {
                            tree::Type *type = new tree::Type(tree::Type::DATA_INT);
                            $$ = new tree::Reference(type, $2);
                        }
                    |   TOKEN_REF type TOKEN_CAST identifier
                        {
                            $$ = new tree::Reference($2, $4);
                        }
                    ;

a_or_e_statement    :   assign_or_expression TOKEN_EOS
                        {
                            $$ = new tree::ExpressionStatement($1);
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
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ASSIGN, $1, $3);
                        }
                    ;

expression_statement:   expression
                        {
                            $$ = new tree::ExpressionStatement($1);
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
                            $$ = new tree::BinaryOperation(tree::Operation::OP_LOGICAL_OR, $1, $3);
                        }
                    ;

l_and_expression    :   or_expression
                        {
                            $$ = $1;
                        }
                    |   l_and_expression TOKEN_LOGICAL_AND or_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_LOGICAL_AND, $1, $3);
                        }
                    ;

or_expression       :   xor_expression
                        {
                            $$ = $1;
                        }
                    |   or_expression TOKEN_OR xor_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_OR, $1, $3);
                        }
                    ;

xor_expression      :   and_expression
                        {
                            $$ = $1;
                        }
                    |   xor_expression TOKEN_XOR and_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_XOR, $1, $3);
                        }
                    ;

and_expression      :   eq_expression
                        {
                            $$ = $1;
                        }
                    |   and_expression TOKEN_AND eq_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_AND, $1, $3);
                        }
                    ;

eq_expression       :   rel_expression
                        {
                            $$ = $1;
                        }
                    |   eq_expression TOKEN_EQ rel_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_EQUAL, $1, $3);
                        }
                    |   eq_expression TOKEN_NE rel_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_UNEQUAL, $1, $3);
                        }
                    ;

rel_expression      :   add_expression
                        {
                            $$ = $1;
                        }
                    |   rel_expression TOKEN_LT add_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_LESS_THAN, $1, $3);
                        }
                    |   rel_expression TOKEN_LE add_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_LESS_EQUAL, $1, $3);
                        }
                    |   rel_expression TOKEN_GT add_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_GREATER_THAN, $1, $3);
                        }
                    |   rel_expression TOKEN_GE add_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_GREATER_EQUAL, $1, $3);
                        }
                    ;

add_expression      :   mult_expression
                        {
                            $$ = $1;
                        }
                    |   add_expression TOKEN_ADD mult_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ADD, $1, $3);
                        }
                    |   add_expression TOKEN_SUBTRACT mult_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_SUBTRACT, $1, $3);
                        }
                    ;

mult_expression     :   cast_expression
                        {
                            $$ = $1;
                        }
                    |   mult_expression TOKEN_MULTIPLY cast_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_MULTIPLY, $1, $3);
                        }
                    |   mult_expression TOKEN_DIVIDE cast_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_DIVIDE, $1, $3);
                        }
                    |   mult_expression TOKEN_MODULUS cast_expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_MODULUS, $1, $3);
                        }
                    ;

cast_expression     :   unary_expression
                        {
                            $$ = $1;
                        }
                    |   type TOKEN_CAST expression_atom
                        {
                            $$ = new tree::CastExpression($1, $3);
                        }
                    ;

unary_expression    :   access_expression
                        {
                            $$ = $1;
                        }
                    |   TOKEN_SUBTRACT access_expression
                        {
                            $$ = new tree::UnaryOperation(tree::Operation::OP_SUBTRACT, $2);
                        }
                    |   TOKEN_LOGICAL_NOT access_expression
                        {
                            $$ = new tree::UnaryOperation(tree::Operation::OP_LOGICAL_NOT, $2);
                        }
                    |   TOKEN_NOT access_expression
                        {
                            $$ = new tree::UnaryOperation(tree::Operation::OP_NOT, $2);
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
                            $$ = new tree::FunctionCall($1, $3.head);
                        }
                    ;

argument_expressions:   expression_statement
                        {
                            $$.head = $1;
                            $$.tail = $1;
                        }
                    |   argument_expressions TOKEN_COMMA expression_statement
                        {
                            $$ = $1;
                            $$.tail->setNext($3);
                            $$.tail = $3;
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
                            $$ = new tree::StringLiteral($1);
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
                            $$ = new tree::Type(tree::Type::DATA_INT);
                        }
                    |   TOKEN_TYPE_FLOAT
                        {
                            $$ = new tree::Type(tree::Type::DATA_FLOAT);
                        }
                    |   TOKEN_TYPE_STRING
                        {
                            $$ = new tree::Type(tree::Type::DATA_STRING);
                        }
                    /*|   TOKEN_NAME
                        {
                            $$ = new tree::UDTType($1);                       // UDTs force this class to be needed?? FIXME
                        }*/
                    ;

identifier          :   TOKEN_ID
                        {
                            $$ = new tree::Identifier($1);
                        }
                    ;

name                :   TOKEN_NAME
                        {
                            $$ = new tree::Identifier($1);
                        }
                    ;

return_statement    :   TOKEN_RETURN expression TOKEN_EOS
                        {
                            $$ = new tree::ReturnStatement($2);
                        }
                    ;

state_statement     :   TOKEN_STATE state TOKEN_EOS /* FIXME, blank state name to reset to default is best?? */
                        {
                            $$ = new tree::StateStatement($2);
                        }
                    ;

state               :   /* Empty (default state) */
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

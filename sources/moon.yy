/*
 * moon.yy
 * Bison grammar for moon
 *
 * Unfortunately, nice-looking Bison is really hard to write. (Although it's easier than Flex...)
 * Please try and match your style to what's already in this file...
 */

%{
    #include <stdio.h>
    #include "error.h"
    #include "tree.h"

    /* Generated headers */
    #include "parser.h"
    #include "lexer.h"

    void yyerror(YYLTYPE *locp, void *scanner, const char *error);
%}

/* Re-entrant */
%pure-parser
%lex-param {void *scanner}
%parse-param {void *scanner}

/* Program locations */
%locations

/* Better errors!! */
%error-verbose

/*  */
%union {
    tree::StatementList *statementList;
    tree::Statement *statement;
    tree::ExpressionList *expressionList;
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
%type<statementList> statement_block
%type<statementList> statements
%type<statement> statement
%type<statement> variable_statement
%type<expression> variable_assignment
%type<expression> variable
%type<statement> reference_statement
%type<expression> reference_assignment
%type<expression> reference
%type<statement> expression_statement
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
%type<statement> return_statement
%type<statement> state_statement
%type<state> state

/* Start symbol */
%start start

%%

start               :   START_PROGRAM program
                    ;

program             :   o_program_includes o_program_uses o_program_cvrs o_program_functions
                        {
                            //yyerror("Test");
                        }
                    ;

o_program_includes  :   /* Empty */
                    |   program_includes
                    ;

program_includes    :   include_statement
                    |   program_includes include_statement
                    ;

include_statement   :   TOKEN_INCLUDE TOKEN_STRING TOKEN_EOS
                        {
                            FILE *input;

                            if((input = fopen($2, "r")))
                            {
                                yyscan_t newScanner;

                                yylex_init(&newScanner);
                                yyset_in(input, newScanner);

                                yyparse(newScanner);

                                yylex_destroy(newScanner);
                                fclose(input);
                            }
                            else
                            {
                                //emitError(locp->first_line, "Could not find include file ...");
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
                    ;

o_program_cvrs      :   /* Empty */
                    |   program_cvrs
                    ;

program_cvrs        :   program_cvr                                                                 /* CVRs = constants + variables + references */
                    |   program_cvrs program_cvr
                        {
                            //$1->setSibling($2);
                        }
                    ;

program_cvr         :   constant_statement
                    |   program_variable
                    |   program_reference
                    ;

constant_statement  :   TOKEN_CONST constant TOKEN_EQUALS expression TOKEN_EOS
                    ;

constant            :   TOKEN_NAME
                        {
                            //tree::Type *type = new tree::Type(tree::type::DATA_INT);
                            //$$ = new tree::Name($1, type);
                        }
                    |   type TOKEN_CAST TOKEN_NAME
                        {
                            //$$ = new tree::Name($3, $1);
                        }
                    ;

program_variable    :   TOKEN_GLOBAL variable_statement
                    |   TOKEN_SHARED variable_statement
                    |   variable_statement
                    ;

program_reference   :   TOKEN_GLOBAL reference_statement
                    |   TOKEN_SHARED reference_statement
                    |   reference_statement
                    ;

o_program_functions :   /* Empty */
                    |   program_functions
                    ;

program_functions   :   program_function
                    |   program_functions program_function
                        {
                            //$1->setSibling($2);
                        }
                    ;

program_function    :   function_prototype statement_block
                        {
                            //$$ = new Function($1, $2);
                        }
                    ;









function_prototype  :   TOKEN_FUNCTION id_type TOKEN_PARENTHESIS_OPEN argument_definitions TOKEN_PARENTHESIS_CLOSE
                        {
                            //$$ = new tree::FunctionPrototype($2, $4);
                        }
                    |   TOKEN_FUNCTION id_type TOKEN_PARENTHESIS_OPEN argument_definitions TOKEN_PARENTHESIS_CLOSE TOKEN_LT state TOKEN_GT
                        {
                            //$$ = new tree::FunctionPrototype($2, $4, $7);
                        }
                    ;

id_type             :   identifier /* FIXME, rename this */
                        {
                            //tree::Type *type = new tree::Type(tree::TYPE_INT);
                            //$$ = new tree::Id($1, type);
                        }
                    |   type TOKEN_CAST identifier
                        {
                            //$$ = new tree::Id($3, $1);
                        }
                    ;

argument_definitions:   /* Empty */
                        {
                            //$$ = null;
                        }
                    |   id_type
                        {
                            //$$ = new tree::IdList();
                            //$$->push($1);
                        }
                    |   argument_definitions TOKEN_COMMA id_type
                        {
                            //$1->push($3);
                        }
                    ;



statement_block     :   TOKEN_BRACE_OPEN TOKEN_BRACE_CLOSE                                          /* Empty... */
                        {
                            $$ = 0;
                        }
                    |   TOKEN_BRACE_OPEN statements TOKEN_BRACE_CLOSE
                        {
                            $$ = $2;
                        }
                    ;

statements          :   statement
                        {
                            $$ = new tree::StatementList();
                            $$->add($1);
                        }
                    |   statements statement
                        {
                            $1->add($2);
                            $$ = $1;
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
                    |   expression_statement
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
                    |   statement_block
                        {
                            $$ = $1;
                        }
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

expression_statement:   assign_or_expression TOKEN_EOS
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
                            $$ = 0; // FIXME
                        }
                    ;

assignment          :   identifier TOKEN_EQUALS expression
                        {
                            $$ = new tree::BinaryOperation(tree::Operation::OP_ASSIGN, $1, $3);
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
                            $$ = new tree::FunctionCall($1, $3);
                        }
                    ;

argument_expressions:   expression
                        {
                            $$ = new tree::ExpressionList();
                            $$->add($1);
                        }
                    |   argument_expressions TOKEN_COMMA expression
                        {
                            $$ = $1;
                            $$->add($3);
                        }
                    ;

expression_atom     :   TOKEN_NAME /* Constant */
                        {
                            $$ = new tree::Identifier($1);
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
                    |   TOKEN_NAME
                        {
                            $$ = new tree::UDTType($1);                       /* UDTs force this class to be needed?? FIXME */
                        }
                    ;

identifier          :   TOKEN_ID
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
                            $$ = 0; /* FIXME */
                        }
                    |   TOKEN_NAME
                        {
                            $$ = new tree::State($1);
                        }
                    ;

%%

void yyerror(YYLTYPE *locp, void *scanner, const char *error)
{
    error::enqueue(locp->first_line, error);
}

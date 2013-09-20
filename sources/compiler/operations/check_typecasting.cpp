#include "common.h"

#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/check_typecasting.h"

bool operation::CheckTypecasting::run(tree::Program *program)
{
	operation::CheckTypecasting operation;
	program->accept(&operation);

	return true;
}


void operation::CheckTypecasting::visit(tree::Cast *cast)
{
	LOG("CheckTypecasting::visit::Cast");

	try
	{
		cast->checkCast();
	}
	catch(tree::Cast::InvalidException &e)
	{
		tree::Expression *expression = cast->getExpression();

		if(tree::node_cast<tree::Void *>(expression->getType()))
		{
			ASSERT(tree::node_cast<tree::FunctionCall *>(expression));

			tree::FunctionCall *functionCall = static_cast<tree::FunctionCall *>(expression);

			ASSERT(tree::node_cast<tree::FunctionPrototype *>(functionCall->getPrototype()));

			std::string error = "Function \"" + static_cast<tree::FunctionPrototype *>(functionCall->getPrototype())->getName() + "\" does not return a value";

			error::enqueue(cast->getLocation(), error);
		}
		else
		{
			std::string error = (cast->getAutoCast() ? "Cannot automatically cast " : "Cannot cast ") + std::string(expression->getType()->getTypeName()) + " to " + std::string(cast->getType()->getTypeName());

			error::enqueue(cast->getLocation(), error);
		}
	}
}

void operation::CheckTypecasting::visit(tree::FunctionCall *functionCall)
{
	tree::Expressions *arguments = functionCall->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; (*i++)->accept(this));
	}
}

void operation::CheckTypecasting::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

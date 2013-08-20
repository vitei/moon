#include "compiler/operations.h"
#include "compiler/tree.h"


class IncompleteException : public std::exception {};

bool operation::CheckTypes::run(tree::Program *program)
{
	try
	{
		operation::CheckTypes operation;
		program->accept(&operation);

		return true;
	}
	catch(IncompleteException &e)
	{
		LOG("New pass required...");
		return false;
	}
}

void operation::CheckTypes::visit(tree::TypedIdentity *typedIdentity)
{
	LOG("CheckTypes::visit::TypedIdentity");

	if(!typedIdentity->getType())
	{
		throw IncompleteException();
	}
}

void operation::CheckTypes::visit(tree::Constant *constant)
{
	LOG("CheckTypes::visit::Constant");

	// There should be no constants left in the tree...
	throw IncompleteException();
}

void operation::CheckTypes::visit(tree::FunctionCall *functionCall)
{
	tree::Expressions *arguments = functionCall->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; (*i++)->accept(this));
	}
}

void operation::CheckTypes::visit(tree::Function *function)
{
	LOG("CheckTypes::visit::Function");

	ASSERT(function->getPrototype());

	tree::Expressions *expressions = function->getPrototype()->getArguments();

	if(expressions)
	{
		for(tree::Expressions::iterator i = expressions->begin(), end = expressions->end(); i != end; (*i++)->accept(this));
	}

	visit(static_cast<tree::Scope *>(function));
}

void operation::CheckTypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

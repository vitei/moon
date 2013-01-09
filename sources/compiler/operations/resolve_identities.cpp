#include <sstream>
#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/resolve_identities.h"


void operation::ResolveIdentities::run(tree::Program *program)
{
	operation::ResolveIdentities operation;
	program->accept(&operation);
}

void operation::ResolveIdentities::visit(tree::Expression *expression)
{
	LOG("ResolveIdentities::visit::Expression");

	int a = mNodeMap.size();

	try
	{
		operation::Restructure::visit(expression);
	}
	catch(tree::FunctionCall::InvalidFunctionException &e)
	{
		std::string error = "The identifier \"" + e.identity->getName() + "\" does not refer to a function";
		error::enqueue(e.identity->getLocation(), error);
	}

	ASSERT(a == mNodeMap.size());
}

tree::Node *operation::ResolveIdentities::restructure(tree::Identifier *identifier)
{
	tree::Node *r = NULL;

	LOG("ResolveIdentities::restructure::Identifier");

	try
	{
		r = getCurrentScope()->findIdentity(identifier);
	}
	catch(tree::Scope::NotFoundException &e)
	{
		std::string error = "The identifier \"" + identifier->getName() + "\" does not exist";
		error::enqueue(identifier->getLocation(), error);
	}

	delete identifier;

	return r;
}

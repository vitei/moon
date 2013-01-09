#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/resolve_identities.h"


void operation::ResolveIdentities::run(tree::Program *program)
{
	operation::ResolveIdentities operation;
	program->accept(&operation);
}

tree::Node *operation::ResolveIdentities::restructure(tree::Identifier *identifier)
{
	tree::Node *r = identifier;

	LOG("ResolveIdentities::restructure::Identifier");

	try
	{
		r = getCurrentScope()->findIdentity(identifier);
		delete identifier;
	}
	catch(tree::Scope::NotFoundException &e)
	{
		std::string error = "The identifier \"" + identifier->getName() + "\" does not exist";
		error::enqueue(identifier->getLocation(), error);
	}

	return r;
}

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

	// FIXME, this is a terrible hack...

	std::stack<tree::Node *> nodeMapClone = mNodeMap;

	for(;;)
	{
		try
		{
			operation::Restructure::visit(static_cast<tree::Node *>(expression));
			break;
		}
		catch(tree::FunctionCall::InvalidFunctionException &e)
		{
			std::string error = "The identifier \"" + e.identity->getName() + "\" does not refer to a function";
			error::enqueue(e.identity->getLocation(), error);

			mNodeMap = nodeMapClone;
		}
		catch(tree::FunctionCall::InvalidArgumentsException &e)
		{
			std::stringstream error;

			error << "The function \"" << e.functionPrototype->getName() << "\" accepts " << e.functionPrototype->getArguments()->size() << " parameters, not " << e.functionCall->getArguments()->size();

			error::enqueue(e.functionPrototype->getLocation(), e.functionCall->getLocation(), error.str());

			mNodeMap = nodeMapClone;
		}
	}
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
		std::string error = "The identifier \"" + e.identifier->getName() + "\" does not exist";
		error::enqueue(e.identifier->getLocation(), error);
	}

	delete identifier;

	return r;
}

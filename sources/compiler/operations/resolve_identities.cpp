#include <sstream>
#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/resolve_identities.h"


void operation::ResolveIdentities::run(tree::Program *program)
{
	operation::ResolveIdentities operation;
	program->accept(&operation);
}

void operation::ResolveIdentities::visit(tree::FunctionCall *functionCall)
{
	operation::Restructure::visit(functionCall);

	LOG("ResolveIdentities::visit::FunctionCall");

	tree::FunctionPrototype *prototype = dynamic_cast<tree::FunctionPrototype *>(functionCall->getFunctionPrototype());

	if(prototype)
	{
		unsigned int numExpectedArguments = prototype->getArguments()->size();
		unsigned int numArguments = functionCall->getArguments()->size();

		if(numExpectedArguments != numArguments)
		{
			std::stringstream error;

			functionCall->setFunctionPrototype(NULL);

			error << "Incorrect number of arguments for function \"" << prototype->getName() << "\"" << std::endl
				<< "\tExpected " << numExpectedArguments << " but got " << numArguments;

			error::enqueue(prototype->getLocation(), functionCall->getLocation(), error.str());
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
		std::string error = "The identifier \"" + identifier->getName() + "\" does not exist";
		error::enqueue(identifier->getLocation(), error);
	}

	delete identifier;

	return r;
}

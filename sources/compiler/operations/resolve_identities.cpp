#include <sstream>
#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


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
			tree::Identifier *identifier = static_cast<tree::Identifier *>(e.functionCall->getPrototype());
			std::string error = "The identifier \"" + identifier->getName() + "\" does not refer to a function";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}
		catch(tree::FunctionCall::InvalidArgumentsException &e)
		{
			tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(e.functionCall->getPrototype());
			std::stringstream error;

			error << "The function \"" << functionPrototype->getName() << "\" accepts " << functionPrototype->getArguments()->size() << " parameters, not " << e.functionCall->getArguments()->size();

			error::enqueue(functionPrototype->getLocation(), e.functionCall->getLocation(), error.str());
			e.reset();
			mNodeMap = nodeMapClone;
		}
		catch(tree::Expression::InvalidException &e)
		{
			ASSERT(e.expression);
			ASSERT(dynamic_cast<tree::Identifier *>(e.expression));

			tree::Identifier *identifier = static_cast<tree::Identifier *>(e.expression);
			std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}
	}
}

void operation::ResolveIdentities::setup(tree::Assign *assign)
{
	if(dynamic_cast<tree::Identifier *>(assign->getLHS()))
	{
		tree::Identifier *identifier = static_cast<tree::Identifier *>(assign->getLHS());

		try
		{
			getCurrentScope()->findIdentity(identifier);
		}
		catch(tree::Scope::NotFoundException &e)
		{
			tree::Variable *variable = new tree::Variable(NULL, identifier->getName());

			variable->setLocation(identifier->getLocation());
			getCurrentScope()->mapIdentity(variable);
		}
	}
}

tree::Node *operation::ResolveIdentities::restructure(tree::Identifier *identifier)
{
	tree::Node *r = NULL;

	LOG("ResolveIdentities::restructure::Identifier");

	try
	{
		identifier->printNode();
		r = getCurrentScope()->findIdentity(identifier);
	}
	catch(tree::Scope::NotFoundException &e)
	{
		std::string error = "The identifier \"" + e.identifier->getName() + "\" does not exist";
		error::enqueue(e.identifier->getLocation(), error);
	}

	//delete identifier; // Can't do this here as it'll cause problems if an exception is thrown...

	return r;
}

tree::Node *operation::ResolveIdentities::restructure(tree::Execute *execute)
{
	LOG("ResolveIdentities::restructure::Execute");

	// If this was just a declaration statement then we can get rid of it.
	// FIXME, when default types are added this will be redundant...
	if(dynamic_cast<tree::Identity *>(execute->getExpression()))
	{
		delete execute;
		return NULL;
	}
	else
	{
		return execute;
	}
}

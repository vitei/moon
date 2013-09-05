#include <sstream>
#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ResolveIdentities::run(tree::Program *program)
{
	operation::ResolveIdentities operation;

	if(program->getStatements())
	{
		operation.add(NULL, program);
		operation.process();
	}
}

void operation::ResolveIdentities::add(tree::Scope *parentScope, tree::Scope *scope)
{
	// We do this so that "shared" functions are placed into their effective scopes when processed.
	mVisitList[parentScope].push(scope);
}

void operation::ResolveIdentities::process()
{
	std::queue<tree::Scope *> processNext;

	processNext.push(NULL);

	while(!processNext.empty())
	{
		tree::Scope *scanScope = processNext.front();
		processNext.pop();

		std::queue<tree::Scope *> &visitNext = mVisitList[scanScope];

		while(!visitNext.empty())
		{
			mCurrentScope = visitNext.front();
			visitNext.pop();

			tree::Function *function = dynamic_cast<tree::Function *>(mCurrentScope);

			if(function)
			{
				processFunctionParameters(function);
			}

			tree::Statements *statements = mCurrentScope->getStatements();

			if(statements)
			{
				for(tree::Statements::iterator i = statements->begin(); i != statements->end();)
				{
					(*i)->accept(this);

					tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
					mNodeMap.pop();

					if(statement)
					{
						*i = statement;
						++i;
					}
					else
					{
						i = statements->erase(i);
					}
				}
			}

			processNext.push(mCurrentScope);
		}
	}
}

void operation::ResolveIdentities::dispatch(tree::Scope *scope)
{
	LOG("ResolveIdentities::dispatch::Scope");

	add(mCurrentScope, scope);
	operation::Restructure::dispatch(static_cast<tree::Statement *>(scope));
}

void operation::ResolveIdentities::dispatch(tree::Function *function)
{
	LOG("ResolveIdentities::dispatch::Function");

	add(function->getOriginalScope() ? function->getOriginalScope() : mCurrentScope, function);
	operation::Restructure::dispatch(static_cast<tree::Statement *>(function));
}

void operation::ResolveIdentities::visit(tree::Identity *identity)
{
	LOG("ResolveIdentities::visit::Identity");

	try
	{
		mCurrentScope->mapIdentity(identity);
	}
	catch(tree::Scope::ExistsException &e)
	{
		std::string error = "The identifier \"" + e.identity->getName() + "\" is already defined";
		error::enqueue(e.conflictingIdentity->getLocation(), e.identity->getLocation(), error);
	}

	operation::Restructure::visit(identity);
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
			tree::Identifier *identifier = (tree::Identifier *)e.functionCall->getPrototype();
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

			tree::Identifier *identifier = (tree::Identifier *)e.expression;
			std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}
	}
}

void operation::ResolveIdentities::visit(tree::Assign *assign)
{
	LOG("ResolveIdentities::visit::Assign");

	// FIXME!!!!
	//static_cast<tree::Expression *>(assign)->childAccept(this);

	if(assign->getRHS())
	{
		assign->getRHS()->accept(this);
	}

	if(assign->getLHS())
	{
		mCanCreateIdentifier = true;
		assign->getLHS()->accept(this);
		mCanCreateIdentifier = false;
	}

	operation::Restructure::visit(static_cast<tree::Expression *>(assign));
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
		if(mCanCreateIdentifier)
		{
			tree::Variable *variable = new tree::Variable(NULL, identifier->getName());

			variable->setLocation(identifier->getLocation());
			getCurrentScope()->mapIdentity(variable);

			r = variable;
		}
		else
		{
			std::string error = "The identifier \"" + e.identifier->getName() + "\" does not exist";
			error::enqueue(e.identifier->getLocation(), error);
		}
	}

	//delete identifier; // Can't do this here as it'll cause problems if an exception is thrown...

	return r;
}

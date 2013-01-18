#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::MapIdentities::run(tree::Program *program)
{
	operation::MapIdentities operation;

	if(program->getStatements())
	{
		operation.add(program);
		operation.process();
	}
}

void operation::MapIdentities::add(tree::Scope *scope)
{
	mVisitNext.push(operation::MapIdentities::ScopeList(scope, scope->getStatements()));
}

void operation::MapIdentities::add(tree::Node *scope, tree::Expressions *expressions)
{
	mVisitNext.push(operation::MapIdentities::ScopeList(scope, expressions));
}

void operation::MapIdentities::add(tree::Node *scope, tree::Statements *statements)
{
	mVisitNext.push(operation::MapIdentities::ScopeList(scope, statements));
}

void operation::MapIdentities::addFunction(tree::Scope *scope)
{
	mVisitFunctions.push(operation::MapIdentities::ScopeList(scope, scope->getStatements()));
}

void operation::MapIdentities::addFunction(tree::Node *scope, tree::Expressions *expressions)
{
	mVisitFunctions.push(operation::MapIdentities::ScopeList(scope, expressions));
}

void operation::MapIdentities::addFunction(tree::Node *scope, tree::Statements *statements)
{
	mVisitFunctions.push(operation::MapIdentities::ScopeList(scope, statements));
}

void operation::MapIdentities::process()
{
	while(!mVisitNext.empty())
	{
		operation::MapIdentities::ScopeList scopeList = mVisitNext.front();
		mVisitNext.pop();

		mCurrentScope = scopeList.scope;

		if(scopeList.expressions)
		{
			for(tree::Expressions::iterator i = scopeList.expressions->begin(), end = scopeList.expressions->end(); i != end; (*i++)->accept(this));
		}
		else
		{
			for(tree::Statements::iterator i = scopeList.statements->begin(), end = scopeList.statements->end(); i != end; (*i++)->accept(this));
		}
	}

	while(!mVisitFunctions.empty())
	{
		operation::MapIdentities::ScopeList scopeList = mVisitFunctions.front();
		mVisitFunctions.pop();

		mCurrentScope = scopeList.scope;

		if(scopeList.expressions)
		{
			for(tree::Expressions::iterator i = scopeList.expressions->begin(), end = scopeList.expressions->end(); i != end; (*i++)->accept(this));
		}
		else
		{
			for(tree::Statements::iterator i = scopeList.statements->begin(), end = scopeList.statements->end(); i != end; (*i++)->accept(this));
		}
	}
}

void operation::MapIdentities::setup(tree::Function *function)
{
	LOG("MapIdentities::setup::Function");

	function->getPrototype()->setFunction(function);
}

void operation::MapIdentities::visit(tree::Function *function)
{
	LOG("MapIdentities::visit::Function");

	tree::FunctionPrototype *prototype = function->getPrototype();
	tree::Expressions *arguments = prototype->getArguments();

	if(arguments)
	{
		addFunction(prototype, arguments);
	}

	if(function->getStatements())
	{
		addFunction(function);
	}
}

void operation::MapIdentities::visit(tree::Scope *scope)
{
	LOG("MapIdentities::visit::Scope");

	if(scope->getStatements())
	{
		add(scope);
	}
}

void operation::MapIdentities::visit(tree::Identity *identity)
{
	LOG("MapIdentities::visit::Identity");

	try
	{
		tree::FunctionPrototype *prototype = dynamic_cast<tree::FunctionPrototype *>(mCurrentScope);

		if(prototype)
		{
			prototype->getFunction()->mapParameterIdentity(identity);
		}
		else
		{
			static_cast<tree::Scope *>(mCurrentScope)->mapIdentity(identity);
		}
	}
	catch(tree::Scope::ExistsException &e)
	{
		std::string error = "The identifier \"" + e.identity->getName() + "\" is already defined";
		error::enqueue(e.conflictingIdentity->getLocation(), e.identity->getLocation(), error);
	}
}

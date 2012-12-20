#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/map_identities.h"


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
	mVisitNext.push(scope);
}

void operation::MapIdentities::process()
{
	while(!mVisitNext.empty())
	{
		tree::Scope *scope = mVisitNext.front();
		mVisitNext.pop();

		if(dynamic_cast<tree::Program *>(scope))
		{
			mProgramScope = scope;
		}
		else if(dynamic_cast<tree::Aggregate *>(scope))
		{
			mAggregateScope = scope;
		}

		mScope = scope;
		mCurrentScope = scope;

		tree::Statements *statements = scope->getStatements();

		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->accept(this);

			tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
			mNodeMap.pop();

			*i = statement;
		}
	}
}

void operation::MapIdentities::setup(tree::Function *function)
{
	LOG("MapIdentities::setup::Function");

	// This is a special case so that prototype variables get added to the correct scope...
	mScope = function;
	mCurrentScope = function;
}

void operation::MapIdentities::setup(tree::GlobalScoping *globalScoping)
{
	LOG("MapIdentities::setup::GlobalScoping");

	mCurrentScope = mProgramScope;
}

void operation::MapIdentities::setup(tree::SharedScoping *sharedScoping)
{
	LOG("MapIdentities::setup::SharedScoping");

	mCurrentScope = mAggregateScope;
}

void operation::MapIdentities::visit(tree::Function *function)
{
	LOG("MapIdentities::visit::Function");

	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
	mNodeMap.pop();

	function->setPrototype(functionPrototype);

	if(function->getStatements())
	{
		add(function);
	}

	// Functions are part of the aggregate
	mCurrentScope = mAggregateScope;

	mNodeMap.push(function->restructure(this));
}

void operation::MapIdentities::visit(tree::Scope *scope)
{
	LOG("MapIdentities::visit::Scope");

	if(scope->getStatements())
	{
		add(scope);
	}

	mNodeMap.push(scope->restructure(this));
}

tree::Node *operation::MapIdentities::restructure(tree::Scope *scope)
{
	LOG("MapIdentities::restructure::Scope");

	return scope;
}

tree::Node *operation::MapIdentities::restructure(tree::GlobalScoping *globalScoping)
{
	LOG("MapIdentities::restructure::GlobalScoping");

	mCurrentScope = mScope;

	tree::Node *r = globalScoping->getScoped();
	delete globalScoping;

	return r;
}

tree::Node *operation::MapIdentities::restructure(tree::SharedScoping *sharedScoping)
{
	LOG("MapIdentities::restructure::SharedScoping");

	mCurrentScope = mScope;

	tree::Node *r = sharedScoping->getScoped();
	delete sharedScoping;

	return r;
}

tree::Node *operation::MapIdentities::restructure(tree::Identity *identity)
{
	LOG("MapIdentities::restructure::Identity");

	try
	{
		mCurrentScope->mapIdentity(identity);
	}
	catch(tree::Scope::ExistsException &e)
	{
		std::string error = "The identifier \"" + identity->getName()->getName() + "\" is already defined";
		error::enqueue(e.identity->getLocation(), identity->getLocation(), error);
	}

	// ... mCurrentScope

	return identity;
}

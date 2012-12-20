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

void operation::MapIdentities::beginScope(tree::Scope *scope)
{
	tree::Program *program;
	tree::Aggregate *aggregate;

	if((program = dynamic_cast<tree::Program *>(scope)))
	{
		mProgramScope = program;
	}
	else if((aggregate = dynamic_cast<tree::Aggregate *>(scope)))
	{
		mAggregateScope = aggregate;
	}

	mCurrentScope = scope;
}

void operation::MapIdentities::setup(tree::Function *function)
{
	LOG("MapIdentities::setup::Function");

	// This is a special case so that prototype variables get added to the correct scope...
	mCurrentScope = function;
}

void operation::MapIdentities::visit(tree::Function *function)
{
	LOG("MapIdentities::visit::Function");

	operation::Restructure::visit(function);
	mCurrentScope = mAggregateScope;
}

void operation::MapIdentities::setup(tree::GlobalScoping *globalScoping)
{
	LOG("MapIdentities::setup::GlobalScoping");

	mCurrentScope = mProgramScope;
}

tree::Node *operation::MapIdentities::restructure(tree::GlobalScoping *globalScoping)
{
	LOG("MapIdentities::restructure::GlobalScoping");

	mCurrentScope = getCurrentScope();

	tree::Node *r = globalScoping->getScoped();
	delete globalScoping;

	return r;
}

void operation::MapIdentities::setup(tree::SharedScoping *sharedScoping)
{
	LOG("MapIdentities::setup::SharedScoping");

	mCurrentScope = mAggregateScope;
}

tree::Node *operation::MapIdentities::restructure(tree::SharedScoping *sharedScoping)
{
	LOG("MapIdentities::restructure::SharedScoping");

	mCurrentScope = getCurrentScope();

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

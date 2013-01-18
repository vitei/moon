#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ScopeParents::run(tree::Program *program)
{
	operation::ScopeParents operation;
	program->accept(&operation);
}

void operation::ScopeParents::setup(tree::Program *program)
{
	LOG("ScopeParents::setup::Program");

	mProgramScope = program;
}

void operation::ScopeParents::setup(tree::Aggregate *aggregate)
{
	LOG("ScopeParents::setup::Aggregate");

	mAggregateScope = aggregate;
}

tree::Node *operation::ScopeParents::restructure(tree::Function *function)
{
	LOG("ScopeParents::restructure::Function");

	function->setParent(getCurrentScope());

	return function;
}

tree::Node *operation::ScopeParents::restructure(tree::Scope *scope)
{
	LOG("ScopeParents::restructure::Scope");

	scope->setParent(getCurrentScope());

	return scope;
}

tree::Node *operation::ScopeParents::restructure(tree::GlobalScoping *globalScoping)
{
	LOG("ScopeParents::restructure::GlobalScoping");

	mProgramScope->getStatements()->push_back(globalScoping->getScoped());
	delete globalScoping;

	return NULL;
}

tree::Node *operation::ScopeParents::restructure(tree::SharedScoping *sharedScoping)
{
	LOG("ScopeParents::restructure::SharedScoping");

	mAggregateScope->getStatements()->push_back(sharedScoping->getScoped());
	delete sharedScoping;

	return NULL;
}

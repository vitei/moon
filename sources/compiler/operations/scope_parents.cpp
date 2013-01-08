#include "compiler/tree.h"
#include "compiler/operations/scope_parents.h"


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

tree::Node *operation::ScopeParents::restructure(tree::GlobalScoping *globalScoping)
{
	LOG("ScopeParents::visit::GlobalScoping");

	mProgramScope->getStatements()->push_back(globalScoping->getScoped());
	delete globalScoping;

	return NULL;
}

tree::Node *operation::ScopeParents::restructure(tree::SharedScoping *sharedScoping)
{
	LOG("ScopeParents::visit::SharedScoping");

	mAggregateScope->getStatements()->push_back(sharedScoping->getScoped());
	delete sharedScoping;

	return NULL;
}

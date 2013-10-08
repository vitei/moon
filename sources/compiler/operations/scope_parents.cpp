#include "common.h"

#include "compiler/tree.h"
#include "compiler/operations/scope_parents.h"

void operation::ScopeParents::run(tree::Program *program)
{
	operation::ScopeParents operation;
	program->accept(&operation);

	ASSERT(operation.mNodeMap.size() == 1);
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

	tree::Function *function = tree::node_cast<tree::Function *>(globalScoping->getScoped());

	if(function)
	{
		function->setOriginalScope(getCurrentScope());
	}

	mProgramScope->getStatements()->push_back(globalScoping->getScoped());
	delete globalScoping;

	return nullptr;
}

tree::Node *operation::ScopeParents::restructure(tree::SharedScoping *sharedScoping)
{
	LOG("ScopeParents::restructure::SharedScoping");

	tree::Function *function = tree::node_cast<tree::Function *>(sharedScoping->getScoped());

	if(function)
	{
		function->setOriginalScope(getCurrentScope());
	}

	mAggregateScope->getStatements()->push_back(sharedScoping->getScoped());
	delete sharedScoping;

	return nullptr;
}

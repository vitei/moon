#include "compiler/tree.h"
#include "compiler/operations/scope_parents.h"


void operation::ScopeParents::run(tree::Program *program)
{
	operation::ScopeParents operation;
	program->dispatch(&operation);
}

void operation::ScopeParents::process(tree::Scope *scope)
{
	LOG("ScopeParents::process::Scope");

	scope->setParent(mScope);

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		mScope = scope;

		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->dispatch(this);
		}

		mScope = scope->getParent();
	}
}

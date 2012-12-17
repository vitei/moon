#include "compiler/tree.h"
#include "compiler/operations/scope_parents.h"


void operation::ScopeParents::run(tree::Program *program)
{
	operation::ScopeParents operation;
	program->accept(&operation);
}

void operation::ScopeParents::visit(tree::Scope *scope)
{
	LOG("ScopeParents::visit::Scope");

	scope->setParent(mScope);

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		mScope = scope;

		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->accept(this);
		}

		mScope = scope->getParent();
	}
}

void operation::ScopeParents::visit(tree::GlobalScoping *globalScoping)
{
	LOG("ScopeParents::visit::Global");
}

void operation::ScopeParents::visit(tree::SharedScoping *sharedScoping)
{
	LOG("ScopeParents::visit::Shared");
}

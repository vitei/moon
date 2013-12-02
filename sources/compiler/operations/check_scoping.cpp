#include "common.h"

#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/check_scoping.h"

void operation::CheckScoping::run(tree::Program *program)
{
	operation::CheckScoping operation;
	program->accept(&operation);

	ASSERT(operation.mLoopDepth == 0);
}

void operation::CheckScoping::setup(tree::For *forStatement)
{
	LOG("CheckScoping::setup::For");

	mLoopDepth++;
}

void operation::CheckScoping::setup(tree::While *whileStatement)
{
	LOG("CheckScoping::setup::While");

	mLoopDepth++;
}

void operation::CheckScoping::visit(tree::Scope *scope)
{
	LOG("CheckScoping::visit::Scope");

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

void operation::CheckScoping::visit(tree::For *forStatement)
{
	LOG("CheckScoping::visit::For");

	mLoopDepth--;
	visit(static_cast<tree::Scope *>(forStatement));
}

void operation::CheckScoping::visit(tree::While *whileStatement)
{
	LOG("CheckScoping::visit::While");

	mLoopDepth--;
}

void operation::CheckScoping::visit(tree::Break *breakStatement)
{
	LOG("CheckScoping::visit::Break");

	if(mLoopDepth == 0)
	{
		error::enqueue(breakStatement->getLocation(), "Break statement not inside a loop");
	}
}

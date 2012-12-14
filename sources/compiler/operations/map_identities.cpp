#include "compiler/tree.h"
#include "compiler/operations/map_identities.h"


void operation::MapIdentities::run(tree::Program *program)
{
	operation::MapIdentities operation;
	program->accept(&operation);
}

void operation::MapIdentities::visit(tree::Scope *scope)
{
	LOG("MapIdentities::visit::Scope");

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->accept(this);
		}
	}
}

void operation::MapIdentities::visit(tree::Identity *identity)
{
	LOG("MapIdentities::visit::Identity");

	LOG("%s", identity->getName()->getName().c_str());
}

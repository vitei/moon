#include "compiler/tree.h"
#include "compiler/operations/map_identities.h"


void operation::MapIdentities::run(tree::Program *program)
{
	operation::MapIdentities operation;
	program->dispatch(&operation);
}

void operation::MapIdentities::process(tree::Scope *scope)
{
	LOG("MapIdentities::process::Scope");

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->dispatch(this);
		}
	}
}

void operation::MapIdentities::process(tree::Identity *identity)
{
	LOG("MapIdentities::process::Identity");

	LOG("%s", identity->getName()->getName().c_str());
}

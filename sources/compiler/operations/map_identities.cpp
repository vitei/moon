#include "compiler/tree.h"
#include "compiler/operations/map_identities.h"


void operation::MapIdentities::run(tree::Program *program)
{
	operation::MapIdentities mapper;
	program->dispatch(&mapper);
}

void operation::MapIdentities::process(tree::Program *program)
{
	LOG("MapIdentities::process::Program");

	tree::Uses *uses = program->getUses();

	for(tree::Uses::iterator i = uses->begin(), end = uses->end(); i != end; ++i)
	{
		(*i)->dispatch(this);
	}
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

void operation::MapIdentities::process(tree::FunctionPrototype *functionPrototype)
{
	LOG("MapIdentities::process::FunctionPrototype");

	LOG("%s", functionPrototype->getName()->getName().c_str());
}

void operation::MapIdentities::process(tree::Variable *variable)
{
	LOG("MapIdentities::process::Variable");

	LOG("%s", variable->getName()->getName().c_str());
}

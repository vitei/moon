#include "compiler/operations.h"
#include "compiler/tree.h"


void tree::Node::accept(operation::Operation *operation)
{
	setup(operation);

	if(operation->doProcessChildren(this))
	{
		childAccept(operation);
	}

	visit(operation);
}

void tree::Node::setup(operation::Operation *operation)
{
	GENERATE_DISPATCH(this, operation->setup)
}

void tree::Node::visit(operation::Operation *operation)
{
	GENERATE_DISPATCH(this, operation->visit)
}

tree::Node *tree::Node::restructure(operation::Restructure *operation)
{
	GENERATE_DISPATCH(this, operation->restructure)
	return NULL;
}

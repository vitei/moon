#include "common.h"

#include "compiler/operations/operation.h"
#include "compiler/operations/restructure.h"
#include "compiler/tree.h"

void tree::Node::accept(operation::Operation *operation)
{
	setup(operation);

	bool canProcessChildren = false;

	GENERATE_ASSIGN(canProcessChildren, this, operation->doProcessChildren)

	if(canProcessChildren)
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

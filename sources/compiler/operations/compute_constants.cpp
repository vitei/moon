#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ComputeConstants::run(tree::Program *program)
{
	operation::ComputeConstants operation;
	program->accept(&operation);
}

tree::Node *operation::ComputeConstants::restructure(tree::Constant *constant)
{
	LOG("ComputeConstants::restructure::Constant");

	return NULL;
}

#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::RemoveDeadNodes::run(tree::Program *program)
{
	operation::RemoveDeadNodes operation;
	program->accept(&operation);
}

tree::Node *operation::RemoveDeadNodes::restructure(tree::Execute *execute)
{
	LOG("RemoveDeadNodes::restructure::Execute");

	return execute->getExpression() && !dynamic_cast<tree::Identity *>(execute->getExpression()) ? execute : NULL;
}

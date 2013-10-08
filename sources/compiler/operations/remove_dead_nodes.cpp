#include "common.h"

#include "compiler/tree.h"
#include "compiler/operations/remove_dead_nodes.h"

void operation::RemoveDeadNodes::run(tree::Program *program)
{
	operation::RemoveDeadNodes operation;
	program->accept(&operation);

	ASSERT(operation.mNodeMap.size() == 1);
}

tree::Node *operation::RemoveDeadNodes::restructure(tree::Execute *execute)
{
	LOG("RemoveDeadNodes::restructure::Execute");

	return execute->getExpression() && !tree::node_cast<tree::Identity *>(execute->getExpression()) ? execute : nullptr;
}

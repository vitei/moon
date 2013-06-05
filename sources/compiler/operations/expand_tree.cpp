#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ExpandTree::run(tree::Program *program)
{
	operation::ExpandTree operation;
	program->accept(&operation);
}

tree::Node *operation::ExpandTree::restructure(tree::OperatorAssign *operatorAssign)
{
	LOG("ExpandTree::restructure::OperatorAssign");

	tree::BinaryExpression *op = operatorAssign->getOperator();

	op->setLHS(operatorAssign->getLHS());
	op->setRHS(operatorAssign->getRHS());

	return new tree::Equals(operatorAssign->getLHS(), op);
}

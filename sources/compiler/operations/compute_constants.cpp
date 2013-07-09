#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ComputeConstants::run(tree::Program *program)
{
	operation::ComputeConstants operation;
	program->accept(&operation);
}

tree::Node *operation::ComputeConstants::restructure(tree::Cast *cast)
{
	LOG("ComputeConstants::restructure::Cast");

	tree::Literal *literal = dynamic_cast<tree::Literal *>(cast->getExpression());

	if(literal)
	{
		tree::Type *type = cast->getType();

		if(dynamic_cast<tree::Bool *>(type))
		{
			return new tree::BoolLiteral(literal);
		}
		else if(dynamic_cast<tree::Int *>(type))
		{
			return new tree::IntLiteral(literal);
		}
		else if(dynamic_cast<tree::Float *>(type))
		{
			return new tree::FloatLiteral(literal);
		}
		/*else if(dynamic_cast<tree::String *>(type))
		{
		}*/
	}

	return cast;
}

tree::Node *operation::ComputeConstants::restructure(tree::BinaryOperation *binaryOperation)
{
	LOG("ComputeConstants::restructure::BinaryOperation");

	tree::Literal *lhsLiteral = dynamic_cast<tree::Literal *>(binaryOperation->getLHS());

	if(lhsLiteral)
	{
		tree::Literal *rhsLiteral = dynamic_cast<tree::Literal *>(binaryOperation->getRHS());

		if(rhsLiteral && *lhsLiteral->getType() == *rhsLiteral->getType())
		{
			return binaryOperation->calculate();
		}
	}

	return binaryOperation;
}

tree::Node *operation::ComputeConstants::restructure(tree::UnaryOperation *unaryOperation)
{
	LOG("ComputeConstants::restructure::UnaryOperation");

	tree::Literal *literal = dynamic_cast<tree::Literal *>(unaryOperation->getExpression());

	if(literal)
	{
		return unaryOperation->calculate();
	}

	return unaryOperation;
}

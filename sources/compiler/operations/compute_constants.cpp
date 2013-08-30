#include "compiler/operations.h"
#include "compiler/tree.h"


bool operation::ComputeConstants::run(tree::Program *program)
{
	operation::ComputeConstants operation;
	program->accept(&operation);

	return true;
}

tree::Node *operation::ComputeConstants::restructure(tree::Assign *assign)
{
	LOG("ComputeConstants::restructure::Assign");

	tree::Constant *constant = dynamic_cast<tree::Constant *>(assign->getLHS());

	if(constant)
	{
		tree::Literal *literal = dynamic_cast<tree::Literal *>(assign->getRHS());

		if(literal)
		{
			constant->setValue(literal);
			return NULL;
		}
	}

	return assign;
}

tree::Node *operation::ComputeConstants::restructure(tree::Cast *cast)
{
	LOG("ComputeConstants::restructure::Cast");

	tree::Literal *literal = dynamic_cast<tree::Literal *>(cast->getExpression());

	if(literal)
	{
		tree::Type *type = cast->getType();
		tree::Bool *boolean;
		tree::Int *integer;
		tree::Float *floatingPoint;
		tree::String *string;

		if((boolean = dynamic_cast<tree::Bool *>(type)))
		{
			return new tree::BoolLiteral(literal, boolean);
		}
		else if((integer = dynamic_cast<tree::Int *>(type)))
		{
			return new tree::IntLiteral(literal, integer);
		}
		else if((floatingPoint = dynamic_cast<tree::Float *>(type)))
		{
			return new tree::FloatLiteral(literal, floatingPoint);
		}
		else if((string = dynamic_cast<tree::String *>(type)))
		{
			return new tree::StringLiteral(literal, string);
		}
	}

	return cast;
}

tree::Node *operation::ComputeConstants::restructure(tree::Constant *constant)
{
	LOG("ComputeConstants::restructure::Constant");

	tree::Literal *literal = constant->getValue();
	
	if(literal)
	{
		return literal;
	}
	else
	{
		return constant;
	}
}

tree::Node *operation::ComputeConstants::restructure(tree::BinaryOperation *binaryOperation)
{
	LOG("ComputeConstants::restructure::BinaryOperation");

	tree::Literal *lhsLiteral = dynamic_cast<tree::Literal *>(binaryOperation->getLHS());

	if(lhsLiteral)
	{
		tree::Literal *rhsLiteral = dynamic_cast<tree::Literal *>(binaryOperation->getRHS());

		ASSERT(lhsLiteral->getType());
		ASSERT(rhsLiteral->getType());

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

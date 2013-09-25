#include "common.h"

#include "compiler/tree.h"
#include "compiler/operations/compute_constants.h"

bool operation::ComputeConstants::run(tree::Program *program)
{
	operation::ComputeConstants operation;

	operation.mValidated = true;
	program->accept(&operation);

	ASSERT(operation.mNodeMap.size() == 1);

	return operation.mValidated;
}

tree::Node *operation::ComputeConstants::restructure(tree::Assign *assign)
{
	LOG("ComputeConstants::restructure::Assign");

	tree::Constant *constant = tree::node_cast<tree::Constant *>(assign->getLHS());

	if(constant)
	{
		tree::Literal *literal = tree::node_cast<tree::Literal *>(assign->getRHS());

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

	tree::Literal *literal = tree::node_cast<tree::Literal *>(cast->getExpression());

	if(literal)
	{
		tree::Type *type = cast->getType();

		if(type && type->isResolved())
		{
			tree::Bool *boolean;
			tree::Int *integer;
			tree::Float *floatingPoint;
			tree::String *string;

			if((boolean = tree::node_cast<tree::Bool *>(type)))
			{
				return new tree::BoolLiteral(literal, boolean);
			}
			else if((integer = tree::node_cast<tree::Int *>(type)))
			{
				return new tree::IntLiteral(literal, integer);
			}
			else if((floatingPoint = tree::node_cast<tree::Float *>(type)))
			{
				return new tree::FloatLiteral(literal, floatingPoint);
			}
			else if((string = tree::node_cast<tree::String *>(type)))
			{
				return new tree::StringLiteral(literal, string);
			}
			else
			{
				ERROR("Should never reach here");
			}
		}
		else
		{
			mValidated = false;
		}
	}

	return cast;
}

tree::Node *operation::ComputeConstants::restructure(tree::IfExpression *ifExpression)
{
	LOG("ComputeConstants::restructure::IfExpression");

	tree::BoolLiteral *value = tree::node_cast<tree::BoolLiteral *>(ifExpression->getTest());

	if(value)
	{
		tree::Type *type = ifExpression->getType();

		if(type && type->isResolved())
		{
			return value->getValue() ? ifExpression->getTrueResult() : ifExpression->getFalseResult();
		}
		else
		{
			mValidated = false;
		}
	}

	return ifExpression;
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

	tree::Literal *lhsLiteral = tree::node_cast<tree::Literal *>(binaryOperation->getLHS());

	if(lhsLiteral)
	{
		tree::Literal *rhsLiteral = tree::node_cast<tree::Literal *>(binaryOperation->getRHS());

		ASSERT(lhsLiteral->getType());

		if(rhsLiteral)
		{
			ASSERT(rhsLiteral->getType());

			tree::Type *lhsType = lhsLiteral->getType();
			tree::Type *rhsType = rhsLiteral->getType();

			if(lhsType && lhsType->isResolved() && rhsType && rhsType->isResolved())
			{
				if(*lhsType == *rhsType)
				{
					return binaryOperation->calculate();
				}
			}
			else
			{
				mValidated = false;
			}
		}
	}

	return binaryOperation;
}

tree::Node *operation::ComputeConstants::restructure(tree::UnaryOperation *unaryOperation)
{
	LOG("ComputeConstants::restructure::UnaryOperation");

	tree::Literal *literal = tree::node_cast<tree::Literal *>(unaryOperation->getExpression());

	if(literal)
	{
		tree::Type *type = literal->getType();

		if(type && type->isResolved())
		{
			return unaryOperation->calculate();
		}
		else
		{
			mValidated = false;
		}
	}

	return unaryOperation;
}

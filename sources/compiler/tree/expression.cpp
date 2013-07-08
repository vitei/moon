#include "compiler/tree.h"


void tree::Access::setContainer(Expression *container)
{
	if(container == NULL)
	{
		mContainer = NULL;
	}
	else if(!dynamic_cast<tree::FunctionPrototype *>(container))
	{
		mContainer = container;
	}
	else
	{
		throw tree::Access::InvalidContainerException(this);
	}
}

void tree::Access::setTarget(Expression *target)
{
	if(target == NULL)
	{
		mTarget = NULL;
	}
	else if(!dynamic_cast<tree::FunctionPrototype *>(target))
	{
		mTarget = target;
	}
	else
	{
		throw tree::Access::InvalidTargetException(this);
	}
}

void tree::UnaryExpression::setExpression(Expression *expression)
{
	if(expression == NULL)
	{
		mExpression = NULL;
	}
	else if(!dynamic_cast<tree::FunctionPrototype *>(expression))
	{
		mExpression = expression;
	}
	else
	{
		throw tree::UnaryExpression::InvalidException(this);
	}
}

void tree::BinaryExpression::setLHS(Expression *lhs)
{
	if(lhs == NULL)
	{
		mLHS = NULL;
	}
	else if(!dynamic_cast<tree::FunctionPrototype *>(lhs))
	{
		mLHS = lhs;
	}
	else
	{
		throw tree::BinaryExpression::InvalidLHSException(this);
	}
}

void tree::BinaryExpression::setRHS(Expression *rhs)
{
	if(rhs == NULL)
	{
		mRHS = NULL;
	}
	else if(!dynamic_cast<tree::FunctionPrototype *>(rhs))
	{
		mRHS = rhs;
	}
	else
	{
		throw tree::BinaryExpression::InvalidRHSException(this);
	}
}

void tree::Cast::checkCast()
{
	tree::Expression *expression = getExpression();

	ASSERT(expression);

	tree::Type *typeB = expression->getType();

	if(typeB)
	{
		tree::Type *typeA = getType();

		ASSERT(typeA);

		if(!typeA->canCast(*typeB, mAutoCast))
		{
			throw tree::Cast::InvalidException(this);
		}
	}
}

void tree::FunctionCall::setPrototype(tree::Expression *prototype)
{
	tree::FunctionPrototype *p;

	if(prototype == NULL)
	{
		mPrototype = NULL;
	}
	else if(dynamic_cast<tree::Identifier *>(prototype))
	{
		mPrototype = prototype;
	}
	else if((p = dynamic_cast<tree::FunctionPrototype *>(prototype)))
	{
		mPrototype = p;

		if((p->getArguments() || mArguments) && ((p->getArguments() && mArguments == NULL) || (p->getArguments() == NULL && mArguments) || p->getArguments()->size() != mArguments->size()))
		{
			throw tree::FunctionCall::InvalidArgumentsException(this);
		}
	}
	else
	{
		throw tree::FunctionCall::InvalidFunctionException(this);
	}
}

tree::BoolLiteral::BoolLiteral(tree::Literal *literal) : Literal(new Bool())
{
	tree::IntLiteral *intLiteral;
	tree::FloatLiteral *floatLiteral;
	tree::StringLiteral *stringLiteral;

	if((intLiteral = dynamic_cast<tree::IntLiteral *>(literal)))
	{
		mValue = (intLiteral->getValue() != 0);
	}
	else if((floatLiteral = dynamic_cast<tree::FloatLiteral *>(literal)))
	{
		mValue = (floatLiteral->getValue() != 0.0f);
	}
	else if((stringLiteral = dynamic_cast<tree::StringLiteral *>(literal)))
	{
		mValue = (stringLiteral->getValue() != "");
	}
	else
	{
		throw tree::BoolLiteral::InvalidException(literal);
	}
}

tree::IntLiteral::IntLiteral(tree::Literal *literal) : Literal(new Int())
{
	tree::BoolLiteral *boolLiteral;
	tree::FloatLiteral *floatLiteral;

	if((boolLiteral = dynamic_cast<tree::BoolLiteral *>(literal)))
	{
		mValue = boolLiteral->getValue() ? 1 : 0;
	}
	else if((floatLiteral = dynamic_cast<tree::FloatLiteral *>(literal)))
	{
		mValue = (int)floatLiteral->getValue();
	}
	else
	{
		throw tree::IntLiteral::InvalidException(literal);
	}
}

tree::FloatLiteral::FloatLiteral(tree::Literal *literal) : Literal(new Float())
{
	tree::BoolLiteral *boolLiteral;
	tree::IntLiteral *intLiteral;

	if((boolLiteral = dynamic_cast<tree::BoolLiteral *>(literal)))
	{
		mValue = boolLiteral->getValue() ? 1.0f : 0.0f;
	}
	else if((intLiteral = dynamic_cast<tree::IntLiteral *>(literal)))
	{
		mValue = (float)intLiteral->getValue();
	}
	else
	{
		throw tree::FloatLiteral::InvalidException(literal);
	}
}

tree::StringLiteral::StringLiteral(tree::Literal *literal) : Literal(new String())
{
	throw tree::StringLiteral::InvalidException(literal);
}

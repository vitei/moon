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
	if(getType() && getExpression())
	{
		if(*getExpression()->getType() > *getType())
		{
			throw tree::Cast::InvalidException(this);
		}
	}
}

void tree::FunctionCall::setFunctionPrototype(tree::Expression *functionPrototype)
{
	tree::FunctionPrototype *prototype;

	if(functionPrototype == NULL)
	{
		mFunctionPrototype = NULL;
	}
	else if(dynamic_cast<tree::Identifier *>(functionPrototype))
	{
		mFunctionPrototype = functionPrototype;
	}
	else if((prototype = dynamic_cast<tree::FunctionPrototype *>(functionPrototype)))
	{
		mFunctionPrototype = prototype;

		if((prototype->getArguments() || mArguments) && ((prototype->getArguments() && mArguments == NULL) || (prototype->getArguments() == NULL && mArguments) || prototype->getArguments()->size() != mArguments->size()))
		{
			throw tree::FunctionCall::InvalidArgumentsException(this);
		}
	}
	else
	{
		throw tree::FunctionCall::InvalidFunctionException(this);
	}
}

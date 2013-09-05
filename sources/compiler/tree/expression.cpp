#include "compiler/tree.h"


void tree::Expression::setType(tree::Type *type)
{
	tree::Node *node = static_cast<tree::Node *>(type); // This is required as the "type" might be an identifier
	tree::Type *t;

	if(type == NULL)
	{
		mType = NULL;
	}
	else if(dynamic_cast<tree::Identifier *>(node))
	{
		mType = type;
	}
	else if((t = dynamic_cast<tree::Type *>(node)))
	{
		mType = t;
	}
	else
	{
		throw tree::Expression::InvalidTypeException(this);
	}
}

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

void tree::Operation::setType(Type *type)
{
	if(type == NULL || type->canPerform(*this))
	{
		tree::Expression::setType(type);
	}
	else
	{
		throw tree::Operation::NotAllowedException(this);
	}
}

void tree::UnaryOperation::setExpression(Expression *expression)
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
		throw tree::UnaryOperation::InvalidException(this);
	}
}

void tree::BinaryOperation::setLHS(Expression *lhs)
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
		throw tree::BinaryOperation::InvalidLHSException(this);
	}
}

void tree::BinaryOperation::setRHS(Expression *rhs)
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
		throw tree::BinaryOperation::InvalidRHSException(this);
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

void tree::FunctionCall::setPrototype(tree::FunctionPrototype *prototype)
{
	tree::Node *node = static_cast<tree::Node *>(prototype); // This is required as the "function prototype" might be an identifier
	tree::FunctionPrototype *p;

	if(prototype == NULL)
	{
		mPrototype = NULL;
	}
	else if(dynamic_cast<tree::Identifier *>(node))
	{
		mPrototype = prototype;
	}
	else if((p = dynamic_cast<tree::FunctionPrototype *>(node)))
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

tree::BoolLiteral::BoolLiteral(tree::Literal *literal, tree::Bool *type) : Literal(type ? type : new Bool())
{
	tree::BoolLiteral *boolLiteral;
	tree::IntLiteral *intLiteral;
	tree::FloatLiteral *floatLiteral;
	tree::StringLiteral *stringLiteral;

	if((boolLiteral = dynamic_cast<tree::BoolLiteral *>(literal)))
	{
		mValue = boolLiteral->getValue();
	}
	else if((intLiteral = dynamic_cast<tree::IntLiteral *>(literal)))
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

bool tree::BoolLiteral::equals(const Literal &literal) const
{
	const tree::BoolLiteral *boolLiteral;

	return (boolLiteral = dynamic_cast<const tree::BoolLiteral *>(&literal)) && mValue == boolLiteral->mValue;
}

tree::IntLiteral::IntLiteral(tree::Literal *literal, tree::Int *type) : NumericLiteral(type ? type : new Int())
{
	tree::BoolLiteral *boolLiteral;
	tree::IntLiteral *intLiteral;
	tree::FloatLiteral *floatLiteral;

	if((boolLiteral = dynamic_cast<tree::BoolLiteral *>(literal)))
	{
		mValue = boolLiteral->getValue() ? 1 : 0;
	}
	else if((intLiteral = dynamic_cast<tree::IntLiteral *>(literal)))
	{
		mValue = intLiteral->getValue();
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

bool tree::IntLiteral::lessThan(const tree::NumericLiteral &numericLiteral) const
{
	const tree::IntLiteral *intLiteral;
	const tree::FloatLiteral *floatLiteral;

	if((intLiteral = dynamic_cast<const tree::IntLiteral *>(&numericLiteral)))
	{
		return mValue < intLiteral->mValue;
	}
	else if((floatLiteral = dynamic_cast<const tree::FloatLiteral *>(&numericLiteral)))
	{
		return mValue < floatLiteral->getValue();
	}

	ASSERT("Should never reach here...");

	return false;
}

bool tree::IntLiteral::equals(const Literal &literal) const
{
	const tree::IntLiteral *intLiteral;

	return (intLiteral = dynamic_cast<const tree::IntLiteral *>(&literal)) && mValue == intLiteral->mValue;
}

tree::FloatLiteral::FloatLiteral(tree::Literal *literal, tree::Float *type) : NumericLiteral(type ? type : new Float())
{
	tree::BoolLiteral *boolLiteral;
	tree::IntLiteral *intLiteral;
	tree::FloatLiteral *floatLiteral;

	if((boolLiteral = dynamic_cast<tree::BoolLiteral *>(literal)))
	{
		mValue = boolLiteral->getValue() ? 1.0f : 0.0f;
	}
	else if((intLiteral = dynamic_cast<tree::IntLiteral *>(literal)))
	{
		mValue = (float)intLiteral->getValue();
	}
	else if((floatLiteral = dynamic_cast<tree::FloatLiteral *>(literal)))
	{
		mValue = floatLiteral->getValue();
	}
	else
	{
		throw tree::FloatLiteral::InvalidException(literal);
	}
}

bool tree::FloatLiteral::lessThan(const tree::NumericLiteral &numericLiteral) const
{
	const tree::IntLiteral *intLiteral;
	const tree::FloatLiteral *floatLiteral;

	if((intLiteral = dynamic_cast<const tree::IntLiteral *>(&numericLiteral)))
	{
		return mValue < intLiteral->getValue();
	}
	else if((floatLiteral = dynamic_cast<const tree::FloatLiteral *>(&numericLiteral)))
	{
		return mValue < floatLiteral->mValue;
	}

	ASSERT("Should never reach here...");

	return false;
}

bool tree::FloatLiteral::equals(const Literal &literal) const
{
	const tree::FloatLiteral *floatLiteral;

	return (floatLiteral = dynamic_cast<const tree::FloatLiteral *>(&literal)) && mValue == floatLiteral->mValue;
}

tree::StringLiteral::StringLiteral(tree::Literal *literal, tree::String *type) : Literal(type ? type : new String())
{
	tree::StringLiteral *stringLiteral;

	if((stringLiteral = dynamic_cast<tree::StringLiteral *>(literal)))
	{
		mValue = stringLiteral->getValue();
	}
	else
	{
		throw tree::StringLiteral::InvalidException(literal);
	}
}

bool tree::StringLiteral::equals(const Literal &literal) const
{
	const tree::StringLiteral *stringLiteral;

	return (stringLiteral = dynamic_cast<const tree::StringLiteral *>(&literal)) && mValue == stringLiteral->mValue;
}

tree::Literal *tree::LogicalOr::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *booleanLHS;

	if((booleanLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *booleanRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(booleanLHS->getValue() || booleanRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::LogicalAnd::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *booleanLHS;

	if((booleanLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *booleanRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(booleanLHS->getValue() && booleanRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Or::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() | intRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Xor::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() ^ intRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Equal::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() == boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() == intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() == floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() == stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Unequal::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() != boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() != intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() != floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() != stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::LessThan::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() < boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() < intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() < floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() < stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::LessEqual::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() <= boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() <= intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() <= floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() <= stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::GreaterThan::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() > boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() > intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() > floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() > stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::GreaterEqual::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::BoolLiteral *boolLHS;
	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;
	tree::StringLiteral *stringLHS;

	if((boolLHS = dynamic_cast<tree::BoolLiteral *>(getLHS())))
	{
		tree::BoolLiteral *boolRHS = static_cast<tree::BoolLiteral *>(getRHS());

		return new tree::BoolLiteral(boolLHS->getValue() >= boolRHS->getValue());
	}
	else if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::BoolLiteral(intLHS->getValue() >= intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::BoolLiteral(floatLHS->getValue() >= floatRHS->getValue());
	}
	else if((stringLHS = dynamic_cast<tree::StringLiteral *>(getLHS())))
	{
		tree::StringLiteral *stringRHS = static_cast<tree::StringLiteral *>(getRHS());

		return new tree::BoolLiteral(stringLHS->getValue() >= stringRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::And::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() & intRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Add::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() + intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::FloatLiteral(floatLHS->getValue() + floatRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Subtract::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() - intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::FloatLiteral(floatLHS->getValue() - floatRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Multiply::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() * intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::FloatLiteral(floatLHS->getValue() * floatRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Divide::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;
	tree::FloatLiteral *floatLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() / intRHS->getValue());
	}
	else if((floatLHS = dynamic_cast<tree::FloatLiteral *>(getLHS())))
	{
		tree::FloatLiteral *floatRHS = static_cast<tree::FloatLiteral *>(getRHS());

		return new tree::FloatLiteral(floatLHS->getValue() / floatRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Modulus::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getLHS()));
	ASSERT(dynamic_cast<tree::Literal *>(getRHS()));
	ASSERT(*getLHS()->getType() == *getLHS()->getType());

	tree::IntLiteral *intLHS;

	if((intLHS = dynamic_cast<tree::IntLiteral *>(getLHS())))
	{
		tree::IntLiteral *intRHS = static_cast<tree::IntLiteral *>(getRHS());

		return new tree::IntLiteral(intLHS->getValue() % intRHS->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::LogicalNot::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getExpression()));

	tree::BoolLiteral *boolExpression;

	if((boolExpression = dynamic_cast<tree::BoolLiteral *>(getExpression())))
	{
		return new tree::BoolLiteral(!boolExpression->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

tree::Literal *tree::Not::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getExpression()));

	tree::IntLiteral *intExpression;

	if((intExpression = dynamic_cast<tree::IntLiteral *>(getExpression())))
	{
		return new tree::IntLiteral(~intExpression->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}

}

tree::Literal *tree::Minus::calculate() const
{
	ASSERT(dynamic_cast<tree::Literal *>(getExpression()));

	tree::IntLiteral *intExpression;
	tree::FloatLiteral *floatExpression;

	if((intExpression = dynamic_cast<tree::IntLiteral *>(getExpression())))
	{
		return new tree::IntLiteral(-intExpression->getValue());
	}
	else if((floatExpression = dynamic_cast<tree::FloatLiteral *>(getExpression())))
	{
		return new tree::FloatLiteral(-floatExpression->getValue());
	}
	else
	{
		ERROR("Invalid type");
	}
}

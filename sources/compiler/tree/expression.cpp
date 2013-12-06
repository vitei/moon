#include "common.h"

#include "compiler/tree/expression.h"
#include "compiler/tree/function.h"
#include "compiler/tree/identifier.h"
#include "compiler/tree/import.h"
#include "compiler/tree/type.h"
#include "compiler/tree/udt.h"

tree::Type *getCastableType(tree::Type *a, tree::Type *b) // FIXME, put this somewhere better?
{
	return a->canCast(*b) ? a : (b->canCast(*a) ? b : nullptr);
}

void tree::Expression::setType(tree::Type *type)
{
	tree::Type *t;

	if(type == nullptr)
	{
		mType = nullptr;
	}
	else if(tree::node_cast<tree::Identifier *>(type) ||
	        tree::node_cast<tree::Type *>(type))
	{
		mType = type;
	}
	else
	{
		throw tree::Expression::InvalidTypeException(this);
	}
}

void tree::Access::setContainer(Expression *container)
{
	if(container == nullptr)
	{
		mContainer = nullptr;
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
	if(target == nullptr)
	{
		mTarget = nullptr;
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

void tree::FunctionPrototype::setTarget(Node *target)
{
	if(target == nullptr)
	{
		mTarget = nullptr;
	}
	else if(   dynamic_cast<tree::Function *>(target)
	        || dynamic_cast<tree::Import *>(target))
	{
		mTarget = target;
	}
	else
	{
		throw tree::FunctionPrototype::InvalidTargetException(this);
	}
}

void tree::UnaryOperation::setExpression(Expression *expression)
{
	if(expression == nullptr)
	{
		mExpression = nullptr;
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

bool tree::UnaryOperation::canCalculateType() const
{
	ASSERT(mExpression);

	tree::Expression *expression = tree::node_cast<tree::Expression *>(mExpression);

	if(expression)
	{
		tree::Type *expressionType = expression->getType();

		return expressionType && expressionType->isResolved();
	}

	return false;
}

void tree::BinaryOperation::setLHS(Expression *lhs)
{
	if(lhs == nullptr)
	{
		mLHS = nullptr;
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
	if(rhs == nullptr)
	{
		mRHS = nullptr;
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

bool tree::BinaryOperation::canCalculateType() const
{
	ASSERT(mLHS);
	ASSERT(mRHS);

	tree::Expression *lhs = tree::node_cast<tree::Expression *>(mLHS);
	tree::Expression *rhs = tree::node_cast<tree::Expression *>(mRHS);

	if(lhs && rhs)
	{
		tree::Type *lhsType = lhs->getType();
		tree::Type *rhsType = rhs->getType();

		return    lhsType && rhsType
		       && lhsType->isResolved() && rhsType->isResolved();
	}

	return false;
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

	if(prototype == nullptr)
	{
		mPrototype = nullptr;
	}
	else if(dynamic_cast<tree::Identifier *>(node))
	{
		mPrototype = prototype;
	}
	else if((p = dynamic_cast<tree::FunctionPrototype *>(node)))
	{
		mPrototype = p;

		if((p->getArguments() || mArguments) && ((p->getArguments() && mArguments == nullptr) || (p->getArguments() == nullptr && mArguments) || p->getArguments()->size() != mArguments->size()))
		{
			throw tree::FunctionCall::InvalidArgumentsException(this);
		}
	}
	else
	{
		throw tree::FunctionCall::InvalidFunctionException(this);
	}
}

bool tree::None::equals(const Literal &literal) const
{
	return dynamic_cast<const tree::None *>(&literal);
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

bool tree::Assign::canCalculateType() const
{
	ASSERT(mLHS);

	tree::Expression *lhs = tree::node_cast<tree::Expression *>(mLHS);

	if(lhs)
	{
		tree::Type *lhsType = lhs->getType();

		return lhsType && lhsType->isResolved();
	}

	return false;
}

tree::Type *tree::Assign::calculateType()
{
	tree::Type *targetType = mLHS->getType();

	if(   dynamic_cast<const tree::Bool *>(targetType)
	   || dynamic_cast<const tree::Int *>(targetType)
	   || dynamic_cast<const tree::Float *>(targetType)
	   || dynamic_cast<const tree::String *>(targetType)
	   || dynamic_cast<const tree::UDT *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::LogicalOr::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Bool *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::LogicalAnd::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Bool *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Or::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Int *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Xor::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Int *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Equal::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType()); // FIXME, this is all wrong for equals stuff...

	if(targetType &&
	   dynamic_cast<const tree::Bool *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Unequal::calculateType()
{
//	tree::Type *targetType = mLHS->getType();

//	if(dynamic_cast<const tree::Bool *>(targetType))
//	{
//		return targetType;
//	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::LessThan::calculateType()
{
//	tree::Type *targetType = mLHS->getType();

//	if(dynamic_cast<const tree::Bool *>(targetType))
//	{
//		return targetType;
//	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::LessEqual::calculateType()
{
//	tree::Type *targetType = mLHS->getType();

//	if(dynamic_cast<const tree::Bool *>(targetType))
//	{
//		return targetType;
//	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::GreaterThan::calculateType()
{
//	tree::Type *targetType = mLHS->getType();

//	if(dynamic_cast<const tree::Bool *>(targetType))
//	{
//		return targetType;
//	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::GreaterEqual::calculateType()
{
//	tree::Type *targetType = mLHS->getType();

//	if(dynamic_cast<const tree::Bool *>(targetType))
//	{
//		return targetType;
//	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::And::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Int *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Add::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   (   dynamic_cast<const tree::Int *>(targetType)
	    || dynamic_cast<const tree::Float *>(targetType)))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Subtract::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   (   dynamic_cast<const tree::Int *>(targetType)
	    || dynamic_cast<const tree::Float *>(targetType)))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Multiply::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   (   dynamic_cast<const tree::Int *>(targetType)
	    || dynamic_cast<const tree::Float *>(targetType)))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Divide::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   (   dynamic_cast<const tree::Int *>(targetType)
	    || dynamic_cast<const tree::Float *>(targetType)))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Modulus::calculateType()
{
	tree::Type *targetType = getCastableType(mLHS->getType(), mRHS->getType());

	if(targetType &&
	   dynamic_cast<const tree::Int *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::LogicalNot::calculateType()
{
	tree::Type *targetType = mExpression->getType();

	if(targetType &&
	   dynamic_cast<const tree::Bool *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Not::calculateType()
{
	tree::Type *targetType = mExpression->getType();

	if(targetType &&
	   dynamic_cast<const tree::Int *>(targetType))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
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

tree::Type *tree::Minus::calculateType()
{
	tree::Type *targetType = mExpression->getType();

	if(targetType &&
	   (   dynamic_cast<const tree::Int *>(targetType)
	    || dynamic_cast<const tree::Float *>(targetType)))
	{
		return targetType;
	}

	throw tree::Operation::NotAllowedException(this);
}

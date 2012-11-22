#ifndef TREE_OPERATION_H
#define TREE_OPERATION_H

#include "expression.h"


namespace tree
{
	class Operation : public Expression
	{
	public:
		enum Type
		{
			OP_ASSIGN,
			OP_LOGICAL_OR,
			OP_LOGICAL_AND,
			OP_OR,
			OP_XOR,
			OP_AND,
			OP_EQUAL,
			OP_UNEQUAL,
			OP_LESS_THAN,
			OP_LESS_EQUAL,
			OP_GREATER_THAN,
			OP_GREATER_EQUAL,
			OP_ADD,
			OP_SUBTRACT,
			OP_MULTIPLY,
			OP_DIVIDE,
			OP_MODULUS,
			OP_CAST_EXPRESSION,
			OP_LOGICAL_NOT,
			OP_NOT
		};

		Operation(Type type) : mType(type) {}

		Type getType()
		{
			return mType;
		}

	private:
		Type mType;
	};

	class UnaryOperation : public Operation
	{
	public:
		UnaryOperation(Operation::Type type, Expression *expression) : Operation(type), mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
		}

	private:
		Expression *mExpression;
	};

	class BinaryOperation : public Operation
	{
	public:
		BinaryOperation(Operation::Type type, Expression *lhs, Expression *rhs) : Operation(type), mLHS(lhs), mRHS(rhs) {}

		Expression *getLHS()
		{
			return mLHS;
		}

		Expression *getRHS()
		{
			return mRHS;
		}

	private:
		Expression *mLHS;
		Expression *mRHS;
	};
}

#endif

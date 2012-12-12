#ifndef COMPILER_TREE_EXPRESSION_H
#define COMPILER_TREE_EXPRESSION_H

#include <string>
#include <vector>
#include "node.h"
#include "type.h"


namespace tree
{
	class Expression : public Node
	{
	};

	typedef std::vector<Expression *> ExpressionList;

	class Identifier : public Expression
	{
	public:
		Identifier(std::string name) : mName(name) {}

		const std::string &getName()
		{
			return mName;
		}

	private:
		std::string mName;
	};

	class Identity : public Expression
	{
	public:
		Identity(Identifier *name) : mName(name) {}

		Identifier *getName()
		{
			return mName;
		}

	private:
		Identifier *mName;
	};

	class TypedIdentity : public Identity
	{
	public:
		TypedIdentity(Type *type, Identifier *name) : Identity(name), mType(type) {}

		Type *getType()
		{
			return mType;
		}

	private:
		Type *mType;
	};

	class Access : public Expression
	{
	public:
		Access(Expression *container, Expression *target) : mContainer(container), mTarget(target) {}

		Expression *getContainer()
		{
			return mContainer;
		}

		Expression *getTarget()
		{
			return mTarget;
		}

	private:
		Expression *mContainer;
		Expression *mTarget;
	};

	class Constant : public TypedIdentity
	{
	public:
		Constant(Type *type, Identifier *name) : TypedIdentity(type, name) {}
	};

	class Variable : public TypedIdentity
	{
	public:
		Variable(Type *type, Identifier *name) : TypedIdentity(type, name) {}
	};

	class Reference : public TypedIdentity
	{
	public:
		Reference(Type *type, Identifier *name) : TypedIdentity(type, name) {}
	};

	class Cast : public Expression
	{
	public:
		Cast(Type *type, Expression *expression) : mType(type), mExpression(expression) {}

		Type *getType()
		{
			return mType;
		}

		Expression *getExpression()
		{
			return mExpression;
		}

	private:
		Type *mType;
		Expression *mExpression;
	};

	class DirectAccess : public Access
	{
	public:
		DirectAccess(Expression *container, Expression *target) : Access(container, target) {}
	};

	class MessageAccess : public Access
	{
	public:
		MessageAccess(Expression *container, Expression *target) : Access(container, target) {}
	};

	class ArrayAccess : public Access
	{
	public:
		ArrayAccess(Expression *array, Expression *index) : Access(array, index) {}
	};

	class FunctionCall : public Expression
	{
	public:
		FunctionCall(Identifier *id, ExpressionList *arguments = NULL) : mID(id), mArguments(arguments) {}

		Identifier *getID()
		{
			return mID;
		}

		ExpressionList *getArguments()
		{
			return mArguments;
		}

	private:
		Identifier *mID;
		ExpressionList *mArguments;
	};

	template<class TYPE, class STORAGE>
	class Literal : public Expression
	{
	public:
		Literal(STORAGE value) : mType(new TYPE()), mValue(value) {}

		Type *getType()
		{
			return mType;
		}

		STORAGE getValue()
		{
			return mValue;
		}

	private:
		Type *mType;
		STORAGE mValue;
	};

	typedef Literal<Int, int> IntLiteral;
	typedef Literal<Float, float> FloatLiteral;
	typedef Literal<String, std::string> StringLiteral;

	class UnaryExpression : public Expression
	{
	public:
		UnaryExpression(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
		}

	private:
		Expression *mExpression;
	};

	class BinaryExpression : public Expression
	{
	public:
		BinaryExpression(Expression *lhs, Expression *rhs) : mLHS(lhs), mRHS(rhs) {}

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

	class Assign : public BinaryExpression
	{
	public:
		Assign(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class LogicalOr : public BinaryExpression
	{
	public:
		LogicalOr(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class LogicalAnd : public BinaryExpression
	{
	public:
		LogicalAnd(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Or : public BinaryExpression
	{
	public:
		Or(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Xor : public BinaryExpression
	{
	public:
		Xor(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class And : public BinaryExpression
	{
	public:
		And(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Equal : public BinaryExpression
	{
	public:
		Equal(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Unequal : public BinaryExpression
	{
	public:
		Unequal(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class LessThan : public BinaryExpression
	{
	public:
		LessThan(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class LessEqual : public BinaryExpression
	{
	public:
		LessEqual(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class GreaterThan : public BinaryExpression
	{
	public:
		GreaterThan(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class GreaterEqual : public BinaryExpression
	{
	public:
		GreaterEqual(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Add : public BinaryExpression
	{
	public:
		Add(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Subtract : public BinaryExpression
	{
	public:
		Subtract(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Multiply : public BinaryExpression
	{
	public:
		Multiply(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Divide : public BinaryExpression
	{
	public:
		Divide(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class Modulus : public BinaryExpression
	{
	public:
		Modulus(Expression *lhs, Expression *rhs) : BinaryExpression(lhs, rhs) {}
	};

	class LogicalNot : public UnaryExpression
	{
	public:
		LogicalNot(Expression *expression) : UnaryExpression(expression) {}
	};

	class Not : public UnaryExpression
	{
	public:
		Not(Expression *expression) : UnaryExpression(expression) {}
	};

	class Minus : public UnaryExpression
	{
	public:
		Minus(Expression *expression) : UnaryExpression(expression) {}
	};
}

#endif

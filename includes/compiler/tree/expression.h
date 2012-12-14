#ifndef COMPILER_TREE_EXPRESSION_H
#define COMPILER_TREE_EXPRESSION_H

#include <string>
#include <vector>
#include "node.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Expression : public Node
	{
	};

	typedef std::vector<Expression *> Expressions;

	/* ---- THE "Identifier" CLASS IS A SPECIAL CASE ---- */

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

	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Identity : public Expression
	{
	public:
		Identity(Identifier *name) : mName(name) {}

		Identifier *getName()
		{
			return mName;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mName->accept(operation);
			visit(operation);
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mType->accept(operation);
			visit(operation);
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mContainer->accept(operation);
			mTarget->accept(operation);
			visit(operation);
		}

	private:
		Expression *mContainer;
		Expression *mTarget;
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mType->accept(operation);
			visit(operation);
		}

	private:
		Type *mType;
		STORAGE mValue;
	};

	class UnaryExpression : public Expression
	{
	public:
		UnaryExpression(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mExpression->accept(operation);
			visit(operation);
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mLHS->accept(operation);
			mRHS->accept(operation);
			visit(operation);
		}

	private:
		Expression *mLHS;
		Expression *mRHS;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mType->accept(operation);
			mExpression->accept(operation);
			visit(operation);
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
		FunctionCall(Identifier *id, Expressions *arguments = NULL) : mID(id), mArguments(arguments) {}

		Identifier *getID()
		{
			return mID;
		}

		Expressions *getArguments()
		{
			return mArguments;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mID->accept(operation);
			visit(operation);
		}

	private:
		Identifier *mID;
		Expressions *mArguments;
	};

	class NullReference : public Expression
	{
	};

	class BoolLiteral : public Literal<Bool, bool>
	{
	public:
		BoolLiteral(bool value) : Literal(value) {}
	};

	class IntLiteral : public Literal<Int, int>
	{
	public:
		IntLiteral(int value) : Literal(value) {}
	};

	class FloatLiteral : public Literal<Float, float>
	{
	public:
		FloatLiteral(float value) : Literal(value) {}
	};

	class StringLiteral : public Literal<String, std::string>
	{
	public:
		StringLiteral(std::string value) : Literal(value) {}
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

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
	protected:
		Expression() { /* Abstract class */ }
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
		Identifier *getName()
		{
			return mName;
		}

		void setName(Identifier *name)
		{
			mName = name;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mName->accept(operation);
			visit(operation);
		}

	protected:
		Identity(Identifier *name) : mName(name) { /* Abstract class */ }

	private:
		Identifier *mName;
	};

	class TypedIdentity : public Identity
	{
	public:
		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mType->accept(operation);
			visit(operation);
		}

	protected:
		TypedIdentity(Type *type, Identifier *name) : Identity(name), mType(type) { /* Abstract class */ }

	private:
		Type *mType;
	};

	class Access : public Expression
	{
	public:
		Expression *getContainer()
		{
			return mContainer;
		}

		void setContainer(Expression *container)
		{
			mContainer = container;
		}

		Expression *getTarget()
		{
			return mTarget;
		}

		void setTarget(Expression *target)
		{
			mTarget = target;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mContainer->accept(operation);
			mTarget->accept(operation);
			visit(operation);
		}

	protected:
		Access(Expression *container, Expression *target) : mContainer(container), mTarget(target) { /* Abstract class */ }

	private:
		Expression *mContainer;
		Expression *mTarget;
	};

	class Literal : public Expression
	{
	public:
		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mType->accept(operation);
			visit(operation);
		}

	protected:
		Literal(Type *type) : mType(type) { /* Abstract class */ }

	private:
		Type *mType;
	};

	class UnaryExpression : public Expression
	{
	public:
		Expression *getExpression()
		{
			return mExpression;
		}

		void setExpression(Expression *expression)
		{
			mExpression = expression;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mExpression->accept(operation);
			visit(operation);
		}

	protected:
		UnaryExpression(Expression *expression) : mExpression(expression) { /* Abstract class */ }

	private:
		Expression *mExpression;
	};

	class BinaryExpression : public Expression
	{
	public:
		Expression *getLHS()
		{
			return mLHS;
		}

		void setLHS(Expression *lhs)
		{
			mLHS = lhs;
		}

		Expression *getRHS()
		{
			return mRHS;
		}

		void setRHS(Expression *rhs)
		{
			mRHS = rhs;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mLHS->accept(operation);
			mRHS->accept(operation);
			visit(operation);
		}

	protected:
		BinaryExpression(Expression *lhs, Expression *rhs) : mLHS(lhs), mRHS(rhs) { /* Abstract class */ }

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

		void setType(Type *type)
		{
			mType = type;
		}

		Expression *getExpression()
		{
			return mExpression;
		}

		void setExpression(Expression *expression)
		{
			mExpression = expression;
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

		void setID(Identifier *id)
		{
			mID = id;
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

	class BoolLiteral : public Literal
	{
	public:
		BoolLiteral(bool value) : Literal(new Bool()), mValue(value) {}

		bool getValue()
		{
			return mValue;
		}

	private:
		bool mValue;
	};

	class IntLiteral : public Literal
	{
	public:
		IntLiteral(int value) : Literal(new Int()), mValue(value) {}

		int getValue()
		{
			return mValue;
		}

	private:
		int mValue;
	};

	class FloatLiteral : public Literal
	{
	public:
		FloatLiteral(float value) : Literal(new Float()), mValue(value) {}

		float getValue()
		{
			return mValue;
		}

	private:
		float mValue;
	};

	class StringLiteral : public Literal
	{
	public:
		StringLiteral(std::string value) : Literal(new String()), mValue(value) {}

		std::string getValue()
		{
			return mValue;
		}

	private:
		std::string mValue;
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

#ifndef TREE_EXPRESSION_H
#define TREE_EXPRESSION_H

#include "node.h"
#include "type.h"


namespace tree
{
	class Statement;

	class Expression : public Node
	{
	};

	class Identifier : public Expression
	{
	public:
		Identifier(const char *name) : mName(std::string(name)) {}

		const std::string &getName()
		{
			return mName;
		}

	private:
		std::string mName;
	};

	class StorageExpression : public Expression
	{
	public:
		StorageExpression(Type *type, Identifier *name) : mType(type), mName(name) {}

		Type *getType()
		{
			return mType;
		}

		Identifier *getName()
		{
			return mName;
		}

	private:
		Type *mType;
		Identifier *mName;
	};

	class Constant : public StorageExpression
	{
	public:
		Constant(Type *type, Identifier *name) : StorageExpression(type, name) {}
	};

	class Variable : public StorageExpression
	{
	public:
		Variable(Type *type, Identifier *name) : StorageExpression(type, name) {}
	};

	class Reference : public StorageExpression
	{
	public:
		Reference(Type *type, Identifier *name) : StorageExpression(type, name) {}
	};

	class CastExpression : public Expression
	{
	public:
		CastExpression(Type *type, Expression *expression) : mType(type), mExpression(expression) {}

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

	class AccessExpression : public Expression
	{
	public:
		AccessExpression(Expression *container, Expression *target) : mContainer(container), mTarget(target) {}

		Expression *getContainer()
		{
			return mContainer;
		}

		Expression *getTarget()
		{
			return mTarget;
		}

	protected:
		Expression *mContainer;
		Expression *mTarget;
	};

	class DirectAccess : public AccessExpression
	{
	public:
		DirectAccess(Expression *container, Expression *target) : AccessExpression(container, target) {}
	};

	class MessageAccess : public AccessExpression
	{
	public:
		MessageAccess(Expression *container, Expression *target) : AccessExpression(container, target) {}
	};

	class ArrayAccess : public AccessExpression
	{
	public:
		ArrayAccess(Expression *array, Expression *index) : AccessExpression(array, index) {}
	};

	class FunctionCall : public Expression
	{
	public:
		FunctionCall(Identifier *id, Statement *firstArgument = 0) : mID(id), mFirstArgument(firstArgument) {}

		Identifier *getID()
		{
			return mID;
		}

		Statement *getFirstArgument()
		{
			return mFirstArgument;
		}

	private:
		Identifier *mID;
		Statement *mFirstArgument;
	};

	template<Type::Data TYPE, class STORAGE>
	class Literal : public Expression
	{
	public:
		Literal(STORAGE value) : mType(new Type(TYPE)), mValue(value) {}

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

	typedef Literal<Type::DATA_INT, int> IntLiteral;
	typedef Literal<Type::DATA_FLOAT, float> FloatLiteral;
	typedef Literal<Type::DATA_STRING, std::string> StringLiteral;
}

#endif

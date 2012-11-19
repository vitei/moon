#ifndef TREE_EXPRESSION_H
#define TREE_EXPRESSION_H

#include <vector>
#include "node.h"
#include "type.h"


namespace tree
{
	class Expression : public Node
	{
	};

	class ExpressionList : public Node
	{
	public:
		void add(Expression *expression)
		{
			mList.push_back(expression);
		}

	private:
		std::vector<Expression *> mList;
	};

	class Identifier : public Expression
	{
	public:
		Identifier(const char *name) : mName(std::string(name)) {}

	private:
		std::string mName;
	};

	class StorageExpression : public Expression
	{
	public:
		StorageExpression(Type *type, Identifier *name) : mType(type), mName(name) {}

	private:
		Type *mType;
		Identifier *mName;
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

	private:
		Type *mType;
		Expression *mExpression;
	};

	class AccessExpression : public Expression
	{
	public:
		AccessExpression(Expression *container, Expression *target) {}

	protected:
		//Identifier *mID;
		//Expression *mInitialValue;
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
		FunctionCall(Identifier *id, ExpressionList *arguments = 0) : mID(id), mArguments(arguments) {}

	private:
		Identifier *mID;
		ExpressionList *mArguments;
	};

	template<Type::Data TYPE, class STORAGE>
	class Literal : public Expression
	{
	public:
		Literal(STORAGE value) : mType(new Type(TYPE)), mValue(value) {}

	private:
		Type *mType;
		STORAGE mValue;
	};

	typedef Literal<Type::DATA_INT, int> IntLiteral;
	typedef Literal<Type::DATA_FLOAT, float> FloatLiteral;
	typedef Literal<Type::DATA_STRING, std::string> StringLiteral;
}

#endif

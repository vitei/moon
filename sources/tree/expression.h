#ifndef TREE_EXPRESSION_H
#define TREE_EXPRESSION_H

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
		std::vector<Expression> mList;
	};

	class Identifier : public Expression
	{
	public:
		Identifier(const char *name) : mName(std::string(name)) {}

	private:
		std::string mName;
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
		FunctionCall(Identifier *id, Expression *arguments = 0) : mID(id), mArguments(arguments) {}

	private:
		Identifier *mID;
		ExpressionList *mArguments;
	};

	template<Type::Data TYPE, class STORAGE>
	class TypeLiteral : public Literal
	{
	public:
		TypeLiteral(STORAGE value) : Literal(new Type(TYPE)), mValue(value) {}

	private:
		STORAGE mValue;
	};

	typedef TypeLiteral<Type::DATA_INT, int> IntLiteral;
	typedef TypeLiteral<Type::DATA_FLOAT, float> FloatLiteral;
	typedef TypeLiteral<Type::DATA_STRING, std::string> StringLiteral;
}

#endif

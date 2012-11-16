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

	class ArrayAccess : public Expression
	{
	public:
		ArrayAccess(Expression *array, Expression *index)/* : mID(id), mInitialValue(initialValue)*/ {}

	private:
		//Identifier *mID;
		//Expression *mInitialValue;
	};

	class FunctionCall : public Expression
	{
	public:
		FunctionCall(Identifier *id, Expression *initialValue = 0) : mID(id), mInitialValue(initialValue) {}

	private:
		Identifier *mID;
		Expression *mInitialValue;
	};

	class Literal : public Expression
	{
	public:
		Literal(Type *dataType) : mDataType(dataType) {}

	private:
		Type *mDataType;
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

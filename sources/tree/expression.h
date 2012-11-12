#ifndef TREE_EXPRESSION_H
#define TREE_EXPRESSION_H

#include "node.h"
#include "type.h"


namespace tree
{
	class Expression : public Node
	{
	};

	class Literal : public Expression
	{
	public:
		Literal(Type::Data dataType) : mDataType(dataType) {}

	private:
		Type::Data mDataType;
	};

	template<Type::Data TYPE, class STORAGE>
	class TypeLiteral : public Literal
	{
	public:
		TypeLiteral(STORAGE value) : Literal(TYPE), mValue(value) {}

	private:
		STORAGE mValue;
	};

	typedef TypeLiteral<Type::DATA_INT, int> IntLiteral;
	typedef TypeLiteral<Type::DATA_FLOAT, float> FloatLiteral;
	typedef TypeLiteral<Type::DATA_STRING, std::string> StringLiteral;

	class Identifier : public Expression
	{
	public:
		Identifier(const char *name) : mName(std::string(name)) {}

	private:
		std::string mName;
	};
}

#endif

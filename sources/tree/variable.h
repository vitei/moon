#ifndef TREE_VARIABLE_H
#define TREE_VARIABLE_H

#include "identifier.h"


namespace tree
{
	class Variable : public Identifier
	{
	public:
		enum Type
		{
			TYPE_INTEGER,
			TYPE_FLOAT
		};

		Variable(Type type) : Identifier(Identifier::TYPE_VARIABLE), mType(type) {}

		Type getVariableType()
		{
			return mType;
		}

	private:
		Type mType;
	};

	class IntegerVariable : public Variable
	{
	public:
		IntegerVariable() : Variable(Variable::TYPE_INTEGER) {}
	};

	class FloatVariable : public Variable
	{
	public:
		FloatVariable() : Variable(Variable::TYPE_FLOAT) {}
	};
}

#endif

#ifndef TREE_FUNCTION_H
#define TREE_FUNCTION_H

#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public Expression
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, NodeList *arguments) : mType(type), mName(name), mArguments(arguments) {}

	private:
		Type *mType;
		Identifier *mName;
		NodeList *mArguments;
	};

	class Function : public Statement
	{
	public:
		Function(FunctionPrototype *prototype, NodeList *statements) : mPrototype(prototype), mStatements(statements) {}

	private:
		FunctionPrototype *mPrototype;
		NodeList *mStatements;
	};
}

#endif

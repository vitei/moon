#ifndef TREE_FUNCTION_H
#define TREE_FUNCTION_H

#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public Expression
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, ExpressionList *arguments) : mType(type), mName(name), mArguments(arguments) {}

	private:
		Type *mType;
		Identifier *mName;
		ExpressionList *mArguments;
	};

	class Function : public Statement
	{
	public:
		Function(FunctionPrototype *prototype, StatementList *statements) : mPrototype(prototype), mStatements(statements) {}

	private:
		FunctionPrototype *mPrototype;
		StatementList *mStatements;
	};
}

#endif

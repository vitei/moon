#ifndef TREE_FUNCTION_H
#define TREE_FUNCTION_H

#include "expression.h"
#include "scope.h"
#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public TypedIdentity
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, ExpressionList *arguments) : TypedIdentity(type, name), mArguments(arguments) {}

		ExpressionList *getArguments()
		{
			return mArguments;
		}

	private:
		ExpressionList *mArguments;
	};

	class Function : public Scope
	{
	public:
		Function(Expression *prototype, StatementList *statements) : Scope(statements), mPrototype(prototype) {}

		Expression *getPrototype()
		{
			return mPrototype;
		}

	private:
		Expression *mPrototype;
	};
}

#endif

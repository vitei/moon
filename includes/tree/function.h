#ifndef TREE_FUNCTION_H
#define TREE_FUNCTION_H

#include "expression.h"
#include "scope.h"
#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public Expression
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, ExpressionList *arguments) : mType(type), mName(name), mArguments(arguments) {}

		Type *getType()
		{
			return mType;
		}

		Identifier *getName()
		{
			return mName;
		}

		ExpressionList *getArguments()
		{
			return mArguments;
		}

	private:
		Type *mType;
		Identifier *mName;
		ExpressionList *mArguments;
	};

	class Function : public Scope
	{
	public:
		Function(FunctionPrototype *prototype, StatementList *statements) : Scope(statements), mPrototype(prototype) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

	private:
		FunctionPrototype *mPrototype;
	};
}

#endif

#ifndef COMPILER_TREE_FUNCTION_H
#define COMPILER_TREE_FUNCTION_H

#include "expression.h"
#include "scope.h"
#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public TypedIdentity
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, Expressions *arguments) : TypedIdentity(type, name), mArguments(arguments) {}

		Expressions *getArguments()
		{
			return mArguments;
		}

	private:
		Expressions *mArguments;
	};

	class Function : public Scope
	{
	public:
		Function(Expression *prototype, Statements *statements) : Scope(statements), mPrototype(prototype) {}

		Expression *getPrototype()
		{
			return mPrototype;
		}

	private:
		Expression *mPrototype;
	};
}

#endif

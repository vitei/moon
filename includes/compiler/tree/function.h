#ifndef COMPILER_TREE_FUNCTION_H
#define COMPILER_TREE_FUNCTION_H

#include "expression.h"
#include "node.h"
#include "scope.h"
#include "statement.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

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

		virtual void dispatch(operation::Operation *operation)
		{
			mPrototype->dispatch(operation);
			Scope::dispatch(operation);
		}

	private:
		Expression *mPrototype;
	};
}

#endif

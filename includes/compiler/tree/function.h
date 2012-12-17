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
		Function(FunctionPrototype *prototype, Statements *statements) : Scope(statements), mPrototype(prototype) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

		void setPrototype(FunctionPrototype *prototype)
		{
			mPrototype = prototype;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mPrototype->accept(operation);
			visit(operation);
		}

	private:
		FunctionPrototype *mPrototype;
	};
}

#endif

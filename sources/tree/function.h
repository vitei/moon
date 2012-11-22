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

		Type *getType()
		{
			return mType;
		}

		Identifier *getName()
		{
			return mName;
		}

		NodeList *getArguments()
		{
			return mArguments;
		}

	private:
		Type *mType;
		Identifier *mName;
		NodeList *mArguments;
	};

	class Function : public Statement
	{
	public:
		Function(FunctionPrototype *prototype, NodeList *statements) : mPrototype(prototype), mStatements(statements) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

		NodeList *getStatements()
		{
			return mStatements;
		}

	private:
		FunctionPrototype *mPrototype;
		NodeList *mStatements;
	};
}

#endif

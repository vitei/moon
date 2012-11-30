#ifndef TREE_FUNCTION_H
#define TREE_FUNCTION_H

#include "statement.h"
#include "type.h"


namespace tree
{
	class FunctionPrototype : public Expression
	{
	public:
		FunctionPrototype(Type *type, Identifier *name, Statement *firstArgument) : mType(type), mName(name), mFirstArgument(firstArgument) {}

		Type *getType()
		{
			return mType;
		}

		Identifier *getName()
		{
			return mName;
		}

		Statement *getFirstArgument()
		{
			return mFirstArgument;
		}

	private:
		Type *mType;
		Identifier *mName;
		Statement *mFirstArgument;
	};

	class Function : public Statement
	{
	public:
		Function(FunctionPrototype *prototype, Statement *firstStatement) : mPrototype(prototype), mFirstStatement(firstStatement) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

		Statement *getFirstStatement()
		{
			return mFirstStatement;
		}

	private:
		FunctionPrototype *mPrototype;
		Statement *mFirstStatement;
	};
}

#endif

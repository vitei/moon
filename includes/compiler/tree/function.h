#ifndef COMPILER_TREE_FUNCTION_H
#define COMPILER_TREE_FUNCTION_H

#include <string>
#include "expression.h"
#include "node.h"
#include "scope.h"
#include "statement.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Function;

	class FunctionPrototype : public TypedIdentity
	{
	public:
		FunctionPrototype(Type *type, std::string name, Expressions *arguments) : TypedIdentity(type, name), mArguments(arguments), mFunction(NULL) {}

		Expressions *getArguments()
		{
			return mArguments;
		}

		Function *getFunction()
		{
			return mFunction;
		}

		void setFunction(Function *function)
		{
			mFunction = function;
		}

	private:
		Expressions *mArguments;
		Function *mFunction;
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

		virtual void childAccept(operation::Operation *operation)
		{
			Scope::childAccept(operation);

			if(mPrototype)
			{
				mPrototype->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	private:
		FunctionPrototype *mPrototype;
	};
}

#endif

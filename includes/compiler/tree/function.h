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

#ifdef DEBUG
		virtual void printNode() { LOG("FunctionPrototype"); }
#endif

	private:
		Expressions *mArguments;
		Function *mFunction;
	};

	class Function : public Scope
	{
	public:
		Function(FunctionPrototype *prototype, Statements *statements) : Scope(statements), mOriginalScope(NULL), mPrototype(prototype) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

		void setPrototype(FunctionPrototype *prototype)
		{
			mPrototype = prototype;
		}

		void setOriginalScope(Scope *originalScope)
		{
			mOriginalScope = originalScope;
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

		virtual void checkIdentity(Identity *identity);
		void mapParameterIdentity(Identity *identity);
		virtual Identity *findIdentity(Identifier *identifier);

#ifdef DEBUG
		virtual void printNode() { LOG("Function"); }
#endif

	private:
		Scope *mOriginalScope;

		FunctionPrototype *mPrototype;
		Identities mParameterIdentities;
	};
}

#endif

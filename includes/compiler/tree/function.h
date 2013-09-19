#ifndef COMPILER_TREE_FUNCTION_H
#define COMPILER_TREE_FUNCTION_H

#include "common.h"

#include <string>
#include "expression.h"
#include "scope.h"
#include "statement.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Function : public Scope
	{
	public:
		Function(FunctionPrototype *prototype, Statements *statements) : Scope(statements), mOriginalScope(NULL), mPrototype(prototype) {}

		FunctionPrototype *getPrototype() const
		{
			return mPrototype;
		}

		void setPrototype(FunctionPrototype *prototype)
		{
			mPrototype = prototype;
		}

		Scope *getOriginalScope() const
		{
			return mOriginalScope;
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

		virtual void checkNamedNode(const std::string &name, Node *node);
		virtual Node *findNamedNode(Identifier *identifier);

#ifdef DEBUG
		virtual void printNode() { LOG("Function"); }
#endif

	private:
		Scope *mOriginalScope;
		FunctionPrototype *mPrototype;
	};
}

#endif

#ifndef COMPILER_TREE_FOR_H
#define COMPILER_TREE_FOR_H

#include "common.h"

#include "node.h"
#include "scope.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class For final : public Scope
	{
	public:
		For(Statements *statements, Variable *variable, Expression *iterable) : Scope(statements), mVariable(variable), mIterable(iterable) {}

		Variable *getVariable() const
		{
			return mVariable;
		}

		void setVariable(Variable *variable)
		{
			mVariable = variable;
		}

		Expression *getIterable() const
		{
			return mIterable;
		}

		void setIterable(Expression *iterable)
		{
			mIterable = iterable;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Scope::childAccept(operation);

			if(mVariable)
			{
				mVariable->accept(operation);
			}

			if(mIterable)
			{
				mIterable->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("For"); }
#endif

	private:
		Variable *mVariable;
		Expression *mIterable;
	};
}

#endif


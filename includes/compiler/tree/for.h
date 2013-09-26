#ifndef COMPILER_TREE_FOR_H
#define COMPILER_TREE_FOR_H

#include "common.h"

#include "node.h"
#include "scope.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class For : public Statement
	{
	public:
		For(Variable *variable, Expression *iterable, Statement *loopStatement) : mVariable(variable), mIterable(iterable), mLoopStatement(loopStatement) {}

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

		Statement *getLoopStatement() const
		{
			return mLoopStatement;
		}

		void setLoopStatement(Statement *loopStatement)
		{
			mLoopStatement = loopStatement;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mVariable)
			{
				mVariable->accept(operation);
			}

			if(mIterable)
			{
				mIterable->accept(operation);
			}

			if(mLoopStatement)
			{
				mLoopStatement->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("For"); }
#endif

	private:
		Variable *mVariable;
		Expression *mIterable;
		Statement *mLoopStatement;
	};
}

#endif


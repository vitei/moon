#ifndef COMPILER_TREE_STATEMENT_H
#define COMPILER_TREE_STATEMENT_H

#include <vector>
#include "node.h"
#include "expression.h"
#include "state.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Statement : public Node
	{
	};

	typedef std::vector<Statement *> Statements;

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Execute : public Statement
	{
	public:
		Execute(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
		}

		virtual void dispatch(operation::Operation *operation)
		{
			mExpression->dispatch(operation);
			Statement::dispatch(operation);
		}

	private:
		Expression *mExpression;
	};

	class Return : public Statement
	{
	public:
		Return(Expression *expression) : mReturn(expression) {}

		Expression *getReturn()
		{
			return mReturn;
		}

		virtual void dispatch(operation::Operation *operation)
		{
			mReturn->dispatch(operation);
			Statement::dispatch(operation);
		}

	private:
		Expression *mReturn;
	};

	class SetState : public Statement
	{
	public:
		SetState(State *state) : mState(state) {}

		State *getState()
		{
			return mState;
		}

		virtual void dispatch(operation::Operation *operation)
		{
			mState->dispatch(operation);
			Statement::dispatch(operation);
		}

	private:
		State *mState;
	};
}

#endif

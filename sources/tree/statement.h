#ifndef TREE_STATEMENT_H
#define TREE_STATEMENT_H

#include <vector>
#include "node.h"
#include "expression.h"
#include "state.h"


namespace tree
{
	class Statement : public Node
	{
	};

	class ExpressionStatement : public Statement
	{
	public:
		ExpressionStatement(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
		}

	private:
		Expression *mExpression;
	};

	class ReturnStatement : public Statement
	{
	public:
		ReturnStatement(Expression *expression) : mReturn(expression) {}

		Expression *getReturn()
		{
			return mReturn;
		}

	private:
		Expression *mReturn;
	};

	class StateStatement : public Statement
	{
	public:
		StateStatement(State *state) : mState(state) {}

		State *getState()
		{
			return mState;
		}

	private:
		State *mState;
	};
}

#endif

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

	typedef std::vector<Statement *> StatementList;

	class Execute : public Statement
	{
	public:
		Execute(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
		{
			return mExpression;
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

	private:
		State *mState;
	};
}

#endif

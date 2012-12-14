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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mExpression->accept(operation);
			visit(operation);
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mReturn->accept(operation);
			visit(operation);
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mState->accept(operation);
			visit(operation);
		}

	private:
		State *mState;
	};
}

#endif

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

	class StatementList : public Statement /* FIXME, should this inherit off statement?? */
	{
	public:
		void add(Statement *statement)
		{
			mList.push_back(statement);
		}

	private:
		std::vector<Statement *> mList;
	};

	class ExpressionStatement : public Statement
	{
	public:
		ExpressionStatement(Expression *expression) {}
	};

	class ReturnStatement : public Statement
	{
	public:
		ReturnStatement(Expression *expression) {}
	};

	class StateStatement : public Statement
	{
	public:
		StateStatement(State *state) : mState(state) {}

	private:
		State *mState;
	};
}

#endif

#ifndef TREE_STATEMENT_H
#define TREE_STATEMENT_H

#include "node.h"
#include "expression.h"


namespace tree
{
	class Statement : public Node
	{
	};

	class StatementList : public Node
	{
	public:
		void add(Statement *statement)
		{
			mList.push_back(statement);
		}

	private:
		std::vector<Statement> mList;
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
		StateStatement(const char *name) : mName(std::string(name)) {}

	private:
		std::string mName;
	};
}

#endif

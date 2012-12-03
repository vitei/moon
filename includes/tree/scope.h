#ifndef TREE_SCOPE_H
#define TREE_SCOPE_H

#include "statement.h"


namespace tree
{
	class Scope : public Statement
	{
	public:
		Scope(Statement *firstStatement) : mFirstStatement(firstStatement) {}

	private:
		Statement *mFirstStatement;
	};
}

#endif

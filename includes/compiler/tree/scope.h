#ifndef COMPILER_TREE_SCOPE_H
#define COMPILER_TREE_SCOPE_H

#include <map>
#include <string>
#include "statement.h"


namespace tree
{
	class Scope : public Statement
	{
	public:
		Scope(StatementList *statements) : mStatements(statements) {}

		std::map<std::string, Identity *> &getIdentities()
		{
			return mIdentities;
		}

		StatementList *getStatements()
		{
			return mStatements;
		}

	private:
		std::map<std::string, Identity *> mIdentities;
		StatementList *mStatements;
	};
}

#endif

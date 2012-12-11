#ifndef TREE_USE_H
#define TREE_USE_H

#include <vector>
#include "scope.h"


namespace tree
{
	class Use : public Scope
	{
	public:
		Use(StatementList *statements) : Scope(statements) {}

	private:
		//ScopeList *mScopeList;
	};

	typedef std::vector<Use *> Uses;
}

#endif

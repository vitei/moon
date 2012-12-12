#ifndef COMPILER_TREE_USE_H
#define COMPILER_TREE_USE_H

#include <vector>
#include "scope.h"


namespace tree
{
	class Use : public Scope
	{
	public:
		Use(StatementList *statements) : Scope(statements) {}
	};

	typedef std::vector<Use *> Uses;
}

#endif

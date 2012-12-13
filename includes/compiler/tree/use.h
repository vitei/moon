#ifndef COMPILER_TREE_USE_H
#define COMPILER_TREE_USE_H

#include <vector>
#include "scope.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Use : public Scope
	{
	public:
		Use(Statements *statements) : Scope(statements) {}
	};

	typedef std::vector<Use *> Uses;
}

#endif
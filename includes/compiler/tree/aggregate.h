#ifndef COMPILER_TREE_AGGREGATE_H
#define COMPILER_TREE_AGGREGATE_H

#include "node.h"
#include "scope.h"
#include "use.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Aggregate : public Scope
	{
	public:
		Aggregate(Statements *uses) : Scope(uses) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Aggregate"); }
#endif
	};
}

#endif

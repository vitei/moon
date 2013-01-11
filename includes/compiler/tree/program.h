#ifndef COMPILER_TREE_PROGRAM_H
#define COMPILER_TREE_PROGRAM_H

#include "node.h"
#include "scope.h"
#include "aggregate.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Program : public Scope
	{
	public:
		Program(Statements *aggregates) : Scope(aggregates) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Program"); }
#endif
	};
}

#endif

#ifndef COMPILER_TREE_UDT_H
#define COMPILER_TREE_UDT_H

#include "expression.h"
#include "node.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class UDT : public Type, public Identity
	{
	public:
		UDT(Identifier *name) : Identity(name) {}
	};
}

#endif

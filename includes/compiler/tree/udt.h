#ifndef COMPILER_TREE_UDT_H
#define COMPILER_TREE_UDT_H

#include "expression.h"
#include "type.h"


namespace tree
{
	class UDT : public Type, Identity
	{
	public:
		UDT(Identifier *name) : Identity(name) {}
	};
}

#endif

#ifndef COMPILER_TREE_USE_H
#define COMPILER_TREE_USE_H

#include "common.h"

#include <vector>
#include "node.h"
#include "scope.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Use final : public NamedScope
	{
	public:
		Use(std::string name, Statements *statements) : NamedScope(name, statements) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Use"); }
#endif
	};
}

#endif

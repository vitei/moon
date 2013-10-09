#ifndef COMPILER_TREE_IDENTIFIER_H
#define COMPILER_TREE_IDENTIFIER_H

#include "common.h"

#include <string>
#include "node.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Identifier final : public Node
	{
	public:
		Identifier(std::string name) : mName(name) {}

		const std::string &getName() const
		{
			return mName;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Identifier: \"%s\"", mName.c_str()); }
#endif

	private:
		std::string mName;
	};
}

#endif

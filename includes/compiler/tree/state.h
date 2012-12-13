#ifndef COMPILER_TREE_STATE_H
#define COMPILER_TREE_STATE_H

#include <string>
#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class State : public Node
	{
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class DefaultState : public State
	{
	};

	class NamedState : public State
	{
	public:
		NamedState(const char *name) : mName(name) {}

		const std::string &getName()
		{
			return mName;
		}

	private:
		std::string mName;
	};
}

#endif

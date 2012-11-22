#ifndef TREE_STATE_H
#define TREE_STATE_H

#include <string>
#include "node.h"


namespace tree
{
	class State : public Node
	{
	};

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

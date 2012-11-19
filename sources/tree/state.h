#ifndef TREE_STATE_H
#define TREE_STATE_H

#include <string>
#include "node.h"


namespace tree
{
	class State : public Node
	{
	public:
		State(const char *name) : mName(name) {}

	private:
		std::string mName;
	};
}

#endif

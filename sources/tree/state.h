#ifndef TREE_STATE_H
#define TREE_STATE_H

#include <string>


namespace tree
{
	class State
	{
	public:
		State(const char *name) : mName(name) {}

	private:
		std::string mName;
	};
}

#endif

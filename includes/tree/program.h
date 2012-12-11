#ifndef TREE_PROGRAM_H
#define TREE_PROGRAM_H

#include "scope.h"
#include "use.h"


namespace tree
{
	class Program : public Scope
	{
	public:

	private:
		Uses *mUses;
	};
}

#endif

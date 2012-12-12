#ifndef TREE_PROGRAM_H
#define TREE_PROGRAM_H

#include "scope.h"
#include "use.h"


namespace tree
{
	class Program : public Scope
	{
	public:

		Program(Uses *uses) : Scope(NULL), mUses(uses) {}

		Uses *getUses()
		{
			return mUses;
		}

	private:
		Uses *mUses;
	};
}

#endif

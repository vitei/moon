#ifndef COMPILER_TREE_PROGRAM_H
#define COMPILER_TREE_PROGRAM_H

#include "scope.h"
#include "use.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Program : public Scope
	{
	public:
		Program(Uses *uses) : Scope(NULL), mUses(uses) {}

		Uses *getUses()
		{
			return mUses;
		}

		void mapIdentities();

	private:
		Uses *mUses;
	};
}

#endif

#ifndef TREE_SCOPING_H
#define TREE_SCOPING_H

#include "expression.h"
#include "node.h"


namespace tree
{
	class Scoping : public Expression
	{
	public:
		Scoping(Identity *scoped) : mScoped(scoped) {}

		Identity *getScoped()
		{
			return mScoped;
		}

	private:
		Identity *mScoped;
	};

	class GlobalScoping: public Scoping
	{
	public:
		GlobalScoping(Identity *scoped) : Scoping(scoped) {}
	};

	class SharedScoping : public Scoping
	{
	public:
		SharedScoping(Identity *scoped) : Scoping(scoped) {}
	};
}

#endif

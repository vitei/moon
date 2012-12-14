#ifndef COMPILER_TREE_SCOPING_H
#define COMPILER_TREE_SCOPING_H

#include "expression.h"
#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Scoping : public Expression
	{
	public:
		Scoping(Identity *scoped) : mScoped(scoped) {}

		Identity *getScoped()
		{
			return mScoped;
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			mScoped->accept(operation);
			visit(operation);
		}

	private:
		Identity *mScoped;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

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

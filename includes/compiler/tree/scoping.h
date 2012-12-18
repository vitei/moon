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
		Identity *getScoped()
		{
			return mScoped;
		}

		void setScoped(Identity *scoped)
		{
			mScoped = scoped;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Expression::childAccept(operation);
			mScoped->accept(operation);
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		Scoping(Identity *scoped) : mScoped(scoped) { /* Abstract class */ }

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

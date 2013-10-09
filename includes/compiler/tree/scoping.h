#ifndef COMPILER_TREE_SCOPING_H
#define COMPILER_TREE_SCOPING_H

#include "common.h"

#include "statement.h"
#include "node.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Scoping : public Statement
	{
	public:
		Statement *getScoped() const
		{
			return mScoped;
		}

		void setScoped(Statement *scoped)
		{
			mScoped = scoped;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mScoped)
			{
				mScoped->accept(operation);
			}
		}

	protected:
		Scoping(Statement *scoped) : mScoped(scoped) { /* Abstract class */ }

	private:
		Statement *mScoped;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class GlobalScoping final : public Scoping
	{
	public:
		GlobalScoping(Statement *scoped) : Scoping(scoped) {}

#ifdef DEBUG
		virtual void printNode() { LOG("GlobalScoping"); }
#endif
	};

	class SharedScoping final : public Scoping
	{
	public:
		SharedScoping(Statement *scoped) : Scoping(scoped) {}

#ifdef DEBUG
		virtual void printNode() { LOG("SharedScoping"); }
#endif
	};
}

#endif

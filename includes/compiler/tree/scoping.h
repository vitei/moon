#ifndef COMPILER_TREE_SCOPING_H
#define COMPILER_TREE_SCOPING_H

#include "statement.h"
#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Scoping : public Statement
	{
	public:
		Statement *getScoped()
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
			mScoped->accept(operation);
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

	protected:
		Scoping(Statement *scoped) : mScoped(scoped) { /* Abstract class */ }

	private:
		Statement *mScoped;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class GlobalScoping: public Scoping
	{
	public:
		GlobalScoping(Statement *scoped) : Scoping(scoped) {}
	};

	class SharedScoping : public Scoping
	{
	public:
		SharedScoping(Statement *scoped) : Scoping(scoped) {}
	};
}

#endif

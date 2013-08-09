#ifndef COMPILER_TREE_IMPORT_H
#define COMPILER_TREE_IMPORT_H

#include "function.h"
#include "node.h"
#include "statement.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Import : public Statement
	{
	public:
		Import(FunctionPrototype *prototype) : mPrototype(prototype) {}

		FunctionPrototype *getPrototype()
		{
			return mPrototype;
		}

		void setPrototype(FunctionPrototype *prototype)
		{
			mPrototype = prototype;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mPrototype)
			{
				mPrototype->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Import"); }
#endif

	private:
		FunctionPrototype *mPrototype;
	};
}

#endif

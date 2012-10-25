#ifndef TREE_BLOCK_H
#define TREE_BLOCK_H

#include <map>
#include <vector>
#include "identifier.h"
#include "reference.h"
#include "statement.h"
#include "variable.h"


namespace tree
{
	class Block : public Statement
	{
	public:
		static void setCurrentBlock(Block *block);
		static Block *getCurrentBlock();


		Block() : mParent(NULL) {}

		Block *getParent()
		{
			return mParent;
		}

		Identifier *getIdentifier(std::string &name)
		{
			std::map<std::string, Identifier *>::iterator i = mIdentifiers.find(name);

			if(i != mIdentifiers.end())
			{
				return i->second;
			}
			else if(mParent)
			{
				return mParent->getIdentifier(name);
			}
			else
			{
				return NULL;
			}
		}

		void addVariable(Variable *variable)
		{
			mIdentifiers[variable->getName()] = variable;
		}

		void addBlock(Block *block)
		{
			mChildStatements.push_back(block);
			block->mParent = this;
		}

	private:
		Block *mParent;

		std::map<std::string, Identifier *> mIdentifiers;
		std::vector<Statement *> mChildStatements;
	};
}

#endif

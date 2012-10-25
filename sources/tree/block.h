#ifndef TREE_BLOCK_H
#define TREE_BLOCK_H

#include <vector>
#include "statement.h"

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

		void addBlock(Block *block)
		{
			mChildStatements.push_back(block);
			block->mParent = this;
		}

	private:
		Block *mParent;
		std::vector<Statement *> mChildStatements;
	};
}

#endif

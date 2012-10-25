#include "block.h"


static tree::Block *mCurrent;

void tree::Block::setCurrentBlock(tree::Block *block)
{
	mCurrent = block;
}

tree::Block *tree::Block::getCurrentBlock()
{
	return mCurrent;
}

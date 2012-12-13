#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node
	{
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Int : public Type
	{
	};

	class Float : public Type
	{
	};

	class String : public Type
	{
	public:
		static const unsigned int STRING_DEFAULT_MAX_SIZE = 256;

		String(unsigned int maxSize = STRING_DEFAULT_MAX_SIZE) : mMaxSize(maxSize) {}

		unsigned int getMaxSize()
		{
			return mMaxSize;
		}

	private:
		unsigned int mMaxSize;
	};
}

#endif

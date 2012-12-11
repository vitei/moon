#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include "node.h"


namespace tree
{
	class Type : public Node
	{
	};

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

	private:
		unsigned int mMaxSize;
	};
}

#endif

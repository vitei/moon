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
	};
}

#endif

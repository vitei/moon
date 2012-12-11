#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include <string>
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

	class UDT : public Type
	{
	public:
		UDT(std::string &typeName) : mTypeName(typeName) {}

	private:
		std::string mTypeName;
	};
}

#endif

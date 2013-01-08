#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node
	{
	protected:
		enum Internal
		{
			TYPE_BOOL,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_STRING,
			TYPE_UDT
		};

		Type(Internal type) : mType(type) { /* Abstract class */ }

	private:
		Internal mType;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Bool : public Type
	{
	public:
		Bool() : Type(tree::Type::TYPE_BOOL) {}
	};

	class Int : public Type
	{
	public:
		Int() : Type(tree::Type::TYPE_INT) {}
	};

	class Float : public Type
	{
	public:
		Float() : Type(tree::Type::TYPE_FLOAT) {}
	};

	class String : public Type
	{
	public:
		static const unsigned int STRING_DEFAULT_MAX_SIZE = 256;

		String(unsigned int maxSize = STRING_DEFAULT_MAX_SIZE) : Type(tree::Type::TYPE_STRING), mMaxSize(maxSize) {}

		unsigned int getMaxSize()
		{
			return mMaxSize;
		}

	private:
		unsigned int mMaxSize;
	};
}

#endif

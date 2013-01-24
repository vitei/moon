#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node
	{
#ifdef DEBUG
	public:
		virtual void printType() = 0;
#endif

	protected:
		enum Internal
		{
			TYPE_VOID,
			TYPE_BOOL,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_UDT,
			TYPE_STRING
		};

		Type(Internal type) : mType(type) { /* Abstract class */ }

#ifdef DEBUG
		virtual void printNode() { LOG("Type"); }
#endif

	private:
		friend bool operator == (Type &type1, Type &type2);
		friend bool operator != (Type &type1, Type &type2);
		friend bool operator < (Type &type1, Type &type2);
		friend bool operator <= (Type &type1, Type &type2);
		friend bool operator > (Type &type1, Type &type2);
		friend bool operator >= (Type &type1, Type &type2);

		Internal mType;
	};

	inline bool operator == (Type &type1, Type &type2)
	{
		return type1.mType == type2.mType;
	}

	inline bool operator != (Type &type1, Type &type2)
	{
		return type1.mType != type2.mType;
	}

	inline bool operator < (Type &type1, Type &type2)
	{
		return type1.mType < type2.mType;
	}

	inline bool operator <= (Type &type1, Type &type2)
	{
		return type1.mType <= type2.mType;
	}

	inline bool operator > (Type &type1, Type &type2)
	{
		return type1.mType > type2.mType;
	}

	inline bool operator >= (Type &type1, Type &type2)
	{
		return type1.mType >= type2.mType;
	}

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Void : public Type
	{
	public:
		Void() : Type(tree::Type::TYPE_VOID) {}

#ifdef DEBUG
		virtual void printType() { LOG("VOID"); }
#endif
	};

	class Bool : public Type
	{
	public:
		Bool() : Type(tree::Type::TYPE_BOOL) {}

#ifdef DEBUG
		virtual void printType() { LOG("BOOL"); }
#endif
	};

	class Int : public Type
	{
	public:
		Int() : Type(tree::Type::TYPE_INT) {}

#ifdef DEBUG
		virtual void printType() { LOG("INT"); }
#endif
	};

	class Float : public Type
	{
	public:
		Float() : Type(tree::Type::TYPE_FLOAT) {}

#ifdef DEBUG
		virtual void printType() { LOG("FLOAT"); }
#endif
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

#ifdef DEBUG
		virtual void printType() { LOG("STRING %d", mMaxSize); }
#endif

	private:
		unsigned int mMaxSize;
	};
}

#endif

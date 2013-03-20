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
		static const unsigned int DEFAULT_SIZE = 32;

		Int(unsigned int Size = DEFAULT_SIZE) : Type(tree::Type::TYPE_INT), mSize(Size) {}

		unsigned int getSize()
		{
			return mSize;
		}

#ifdef DEBUG
		virtual void printType() { LOG("INT %d", mSize); }
#endif

	private:
		unsigned int mSize;
	};

	class Float : public Type
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Float(unsigned int Size = DEFAULT_SIZE) : Type(tree::Type::TYPE_FLOAT), mSize(Size) {}

		unsigned int getSize()
		{
			return mSize;
		}

#ifdef DEBUG
		virtual void printType() { LOG("FLOAT %d", mSize); }
#endif

	private:
		unsigned int mSize;
	};

	class String : public Type
	{
	public:
		static const unsigned int DEFAULT_SIZE = 256;

		String(unsigned int Size = DEFAULT_SIZE) : Type(tree::Type::TYPE_STRING), mSize(Size) {}

		unsigned int getSize()
		{
			return mSize;
		}

#ifdef DEBUG
		virtual void printType() { LOG("STRING %d", mSize); }
#endif

	private:
		unsigned int mSize;
	};
}

#endif

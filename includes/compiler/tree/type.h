#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include "node.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node
	{
	public:
#ifdef DEBUG
		virtual void printNode() { LOG("Type"); }
		virtual void printType() = 0;
#endif

		virtual const char *getTypeName() = 0;

	protected:
		enum Internal
		{
			TYPE_VOID,
			TYPE_BOOL,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_UDT,
			TYPE_STRING,
			TYPE_ARRAY
		};

		Type(Internal type) : mType(type) { /* Abstract class */ }

	private:
		friend bool operator == (const Type &type1, const Type &type2);
		friend bool operator != (const Type &type1, const Type &type2);
		friend bool operator < (const Type &type1, const Type &type2);
		friend bool operator <= (const Type &type1, const Type &type2);
		friend bool operator > (const Type &type1, const Type &type2);
		friend bool operator >= (const Type &type1, const Type &type2);

		Internal mType;
	};

	inline bool operator == (const Type &type1, const Type &type2)
	{
		return type1.mType == type2.mType;
	}

	inline bool operator != (const Type &type1, const Type &type2)
	{
		return type1.mType != type2.mType;
	}

	inline bool operator < (const Type &type1, const Type &type2)
	{
		return type1.mType < type2.mType;
	}

	inline bool operator <= (const Type &type1, const Type &type2)
	{
		return type1.mType <= type2.mType;
	}

	inline bool operator > (const Type &type1, const Type &type2)
	{
		return type1.mType > type2.mType;
	}

	inline bool operator >= (const Type &type1, const Type &type2)
	{
		return type1.mType >= type2.mType;
	}

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Void : public Type
	{
	public:
		Void() : Type(tree::Type::TYPE_VOID) {}

		virtual const char *getTypeName()
		{
			return "void";
		}

#ifdef DEBUG
		virtual void printType() { LOG("VOID"); }
#endif
	};

	class Bool : public Type
	{
	public:
		Bool() : Type(tree::Type::TYPE_BOOL) {}

		virtual const char *getTypeName()
		{
			return "boolean";
		}

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

		virtual const char *getTypeName()
		{
			return "integer";
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

		virtual const char *getTypeName()
		{
			return "floating point";
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

		virtual const char *getTypeName()
		{
			return "string";
		}

#ifdef DEBUG
		virtual void printType() { LOG("STRING %d", mSize); }
#endif

	private:
		unsigned int mSize;
	};

	class Array : public Type
	{
	public:
		static const long long UNDEFINED_SIZE = -1;

		Array(Type *type, long long size = UNDEFINED_SIZE) : Type(tree::Type::TYPE_ARRAY), mType(type), mSize(size) {}

		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		long long getSize()
		{
			return mSize;
		}

		virtual const char *getTypeName()
		{
			return "array"; // FIXME
		}

#ifdef DEBUG
		virtual void printType() { LOG("Array[%lld]", mSize); if(mType) { mType->printType(); } }
#endif

	private:
		Type *mType;
		long long mSize;
	};
}

#endif

#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include <string>
#include <sstream>
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

		virtual const char *getTypeName() const = 0;

		virtual bool canCast(const Type &from) const
		{
			return false;
		}

	protected:
		Type() { /* Abstract class */ }

		virtual bool equals(const Type &type) const = 0;

	private:
		friend bool operator == (const Type &type1, const Type &type2);
	};

	inline bool operator == (const Type &type1, const Type &type2)
	{
		return type1.equals(type2);
	}

	inline bool operator != (const Type &type1, const Type &type2)
	{
		return !(type1 == type2);
	}

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Void : public Type
	{
	public:
		Void() {}

		virtual const char *getTypeName() const
		{
			return "void";
		}

#ifdef DEBUG
		virtual void printType() { LOG("VOID"); }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			return dynamic_cast<const Void *>(&type) != 0;
		}
	};

	class Bool : public Type
	{
	public:
		Bool() {}

		virtual const char *getTypeName() const
		{
			return "boolean";
		}

#ifdef DEBUG
		virtual void printType() { LOG("BOOL"); }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			return dynamic_cast<const Bool *>(&type) != 0;
		}
	};

	class Int : public Type
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Int(unsigned int Size = DEFAULT_SIZE) : mSize(Size) {}

		bool canCast(const Type &from) const;

		unsigned int getSize()
		{
			return mSize;
		}

		virtual const char *getTypeName() const
		{
			return "integer";
		}

#ifdef DEBUG
		virtual void printType() { LOG("INT %d", mSize); }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			const Int *integer = dynamic_cast<const Int *>(&type);

			if(integer)
			{
				return mSize == integer->mSize;
			}
			else
			{
				return false;
			}
		}

	private:
		unsigned int mSize;
	};

	class Float : public Type
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Float(unsigned int Size = DEFAULT_SIZE) : mSize(Size) {}

		bool canCast(const Type &from) const;

		unsigned int getSize()
		{
			return mSize;
		}

		virtual const char *getTypeName() const
		{
			return "floating point";
		}

#ifdef DEBUG
		virtual void printType() { LOG("FLOAT %d", mSize); }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			const Float *floatingPoint = dynamic_cast<const Float *>(&type);

			if(floatingPoint)
			{
				return mSize == floatingPoint->mSize;
			}
			else
			{
				return false;
			}
		}

	private:
		unsigned int mSize;
	};

	class String : public Type
	{
	public:
		static const unsigned int DEFAULT_SIZE = 256;

		String(unsigned int Size = DEFAULT_SIZE) : mSize(Size) {}

		bool canCast(const Type &from) const;

		unsigned int getSize()
		{
			return mSize;
		}

		virtual const char *getTypeName() const
		{
			return "string";
		}

#ifdef DEBUG
		virtual void printType() { LOG("STRING %d", mSize); }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			const String *string = dynamic_cast<const String *>(&type);

			if(string)
			{
				return true;//mSize == string->mSize;
			}
			else
			{
				return false;
			}
		}

	private:
		unsigned int mSize;
	};

	class Array : public Type
	{
	public:
		static const unsigned long UNDEFINED_SIZE = 0;

		Array(Type *type, unsigned long size = UNDEFINED_SIZE) : mType(type), mSize(size) {}

		bool canCast(const Type &from) const;

		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		unsigned long getSize()
		{
			return mSize;
		}

		virtual const char *getTypeName() const
		{
			std::stringstream typeName;

			tree::Type *baseType;

			for(const tree::Array *i = this; i; baseType = i->mType, i = dynamic_cast<const tree::Array *>(baseType))
				;

			typeName << baseType->getTypeName();

			for(const tree::Array *i = this; i; i = dynamic_cast<const tree::Array *>(i->mType))
			{
				typeName << "[" << i->mSize << "]";
			}

			return typeName.str().c_str(); // FIXME, is this unsafe??
		}

#ifdef DEBUG
		virtual void printType() { LOG("Array[%lu]", mSize); if(mType) { mType->printType(); } }
#endif

	protected:
		virtual bool equals(const Type &type) const
		{
			const Array *array = dynamic_cast<const Array *>(&type);

			if(array)
			{
				return mSize == array->mSize && (*mType == *array->mType);
			}
			else
			{
				return false;
			}
		}

	private:
		Type *mType;
		unsigned long mSize;
	};
}

#endif

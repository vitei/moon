#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include <string>
#include <sstream>
#include "node.h"


namespace tree
{
	class Expression;
	class IntLiteral;
	class Operation;

	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node
	{
	public:
		class InvalidException : public std::exception
		{
		public:
			Type *type;

		protected:
			InvalidException(Type *_type) : type(_type) { /* Abstract class */ }
		};

#ifdef DEBUG
		virtual void printNode() { LOG("Type"); }
		virtual void printType() = 0;
#endif

		virtual const char *getTypeName() const = 0; // FIXME, typeName() ???

		virtual bool canCast(const Type &from, bool autoCast = false) const
		{
			return false;
		}

		virtual bool canPerform(const Operation &operation) const
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

	class SizedType : public Type
	{
	public:
		Expression *getSize() const
		{
			return mSize.expression;
		}

		void setSize(Expression *size)
		{
			mSize.expression = size;
		}

		virtual unsigned int getSizeInt() const = 0;
		virtual bool isResolved() const;

		virtual void childAccept(operation::Operation *operation);

	protected:
		SizedType(Expression *size = NULL) { mSize.expression = size; }

		union
		{
			Expression *expression;
			IntLiteral *intLiteral;
		} mSize;
	};

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
		virtual bool equals(const Type &type) const;
	};

	class Bool : public Type
	{
	public:
		Bool() {}

		bool canCast(const Type &from, bool autoCast = false) const;
		bool canPerform(const Operation &operation) const;

		virtual const char *getTypeName() const
		{
			return "boolean";
		}

#ifdef DEBUG
		virtual void printType() { LOG("BOOL"); }
#endif

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Int : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Int(Expression *size = NULL) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;
		bool canPerform(const Operation &operation) const;

		virtual const char *getTypeName() const
		{
			return "integer";
		}

#ifdef DEBUG
		virtual void printType() { LOG("INT %d", /*mSize*/0); } // FIXME
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Float : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Float(Expression *size = NULL) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;
		bool canPerform(const Operation &operation) const;

		virtual const char *getTypeName() const
		{
			return "floating point";
		}

#ifdef DEBUG
		virtual void printType() { LOG("FLOAT %d", /*mSize*/0); } // FIXME
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class String : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		String(Expression *size = NULL) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		virtual const char *getTypeName() const
		{
			return "string";
		}

#ifdef DEBUG
		virtual void printType() { LOG("STRING %d", /*mSize*/0); } // FIXME
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Array : public SizedType
	{
	public:
		/*class InvalidSizeException : public tree::Type::InvalidException
		{
		public:
			InvalidSizeException(Type *_type) : InvalidException(_type) {}
		};*/

		//Array(Type *type);
		Array(Type *type, Expression *size) : SizedType(size), mType(type) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		Type *getType()
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
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
				////typeName << "[" << i->mSize << "]"; FIXME
			}

			return typeName.str().c_str(); // FIXME, is this unsafe??
		}

#ifdef DEBUG
		virtual void printType() { LOG("Array[%u]", /*mSize*/0); if(mType) { mType->printType(); } } // FIXME
#endif

		virtual unsigned int getSizeInt() const;

		virtual void childAccept(operation::Operation *operation);

	protected:
		virtual bool equals(const Type &type) const;

	private:
		Type *mType;
	};
}

#endif

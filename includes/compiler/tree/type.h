#ifndef COMPILER_TREE_TYPE_H
#define COMPILER_TREE_TYPE_H

#include "common.h"

#include <string>
#include <sstream>
#include "compiler/behaviours/named_map.h"
#include "node.h"

namespace tree
{
	class Expression;
	class Literal;
	class None;
	class IntLiteral;
	class Operation;

	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Type : public Node, public behaviour::NamedMap
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

		virtual const std::string getTypeName() const = 0; // FIXME, typeName() ???

		virtual bool canCast(const Type &from, bool autoCast = false) const
		{
			return false;
		}

		virtual bool isResolved() const
		{
			return true;
		}

		virtual void checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node);
		virtual tree::Node *findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier);
		virtual void mapAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node);

	protected:
		Type() { /* Abstract class */ }

		virtual bool equals(const Type &type) const = 0;

	private:
		friend bool operator == (const Type &type1, const Type &type2);

		behaviour::NamedMap::AssociatedNamedNodes mAssociatedNamedNodes;
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
		class InvalidException final : public tree::Type::InvalidException
		{
		public:
			InvalidException(SizedType *_type) : tree::Type::InvalidException(_type) {}

			virtual void reset()
			{
				LOG("tree::SizedType::InvalidException::reset");
				static_cast<tree::SizedType *>(type)->setSize(nullptr);
			}
		};

		Expression *getSize() const
		{
			return mSize.expression;
		}

		void setSize(Expression *size);

		virtual bool isResolved() const;

		virtual Literal *getSizeLiteral() const
		{
			return mSize.literal;
		}

		virtual unsigned int getSizeInt() const = 0;

		virtual void childAccept(operation::Operation *operation);

	protected:
		SizedType(Expression *size = nullptr) { mSize.expression = size; }

		union
		{
			Expression *expression;
			Literal *literal;
			None *none;
			IntLiteral *intLiteral;
		} mSize;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Void final : public Type
	{
	public:
		Void() {}

		virtual const std::string getTypeName() const
		{
			return "void";
		}

#ifdef DEBUG
		virtual void printType() { LOG("VOID"); }
#endif

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Bool final : public Type
	{
	public:
		Bool() {}

		bool canCast(const Type &from, bool autoCast = false) const;

		virtual const std::string getTypeName() const
		{
			return "boolean";
		}

#ifdef DEBUG
		virtual void printType() { LOG("BOOL"); }
#endif

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Int final : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Int(Expression *size = nullptr) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		virtual const std::string getTypeName() const
		{
			std::stringstream typeName;

			typeName << getSizeInt() << " bit integer";

			return typeName.str();
		}

#ifdef DEBUG
		virtual void printType() { LOG("INT<%d>", getSizeInt()); }
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Float final : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		Float(Expression *size = nullptr) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		virtual const std::string getTypeName() const
		{
			std::stringstream typeName;

			typeName << getSizeInt() << " bit floating point";

			return typeName.str();
		}

#ifdef DEBUG
		virtual void printType() { LOG("FLOAT<%d>", getSizeInt()); }
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class String final : public SizedType
	{
	public:
		static const unsigned int DEFAULT_SIZE = 32;

		String(Expression *size = nullptr) : SizedType(size) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		virtual const std::string getTypeName() const
		{
			std::stringstream typeName;

			typeName << getSizeInt() << " byte string";

			return typeName.str();
		}

#ifdef DEBUG
		virtual void printType() { LOG("STRING<%d>", getSizeInt()); }
#endif

		virtual unsigned int getSizeInt() const;

	protected:
		virtual bool equals(const Type &type) const;
	};

	class Array final : public SizedType
	{
	public:
		/*class InvalidSizeException : public tree::Type::InvalidException
		{
		public:
			InvalidSizeException(Type *_type) : InvalidException(_type) {}
		};*/

		Array(Type *type);
		Array(Type *type, Expression *size) : SizedType(size), mType(type) {}

		bool canCast(const Type &from, bool autoCast = false) const;

		Type *getType() const
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		virtual const std::string getTypeName() const
		{
			std::stringstream typeName;

			tree::Type *baseType;

			for(const tree::Array *i = this; i; baseType = i->mType, i = dynamic_cast<const tree::Array *>(baseType))
				;

			typeName << baseType->getTypeName();

			for(const tree::Array *i = this; i; i = dynamic_cast<const tree::Array *>(i->mType))
			{
				typeName << "[" << i->getSizeInt() << "]";
			}

			return typeName.str();
		}

#ifdef DEBUG
		virtual void printType() { LOG("Array[%u]", getSizeInt()); if(mType) { mType->printType(); } } // FIXME
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

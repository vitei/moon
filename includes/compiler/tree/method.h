#ifndef COMPILER_TREE_METHOD_H
#define COMPILER_TREE_METHOD_H

#include "common.h"

#include <string>
#include "function.h"
#include "type.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Method : public Function
	{
	public:
		class InvalidException : public std::exception
		{
		public:
			virtual void reset() = 0;

			Method *method;

		protected:
			InvalidException(Method *_method) : method(_method) { /* Abstract class */ }
		};

		class InvalidTypeException : public InvalidException
		{
		public:
			InvalidTypeException(Method *_method) : InvalidException(_method) {}

			virtual void reset()
			{
				LOG("tree::Method::InvalidTypeException::reset");
				method->setType(NULL);
			}
		};

		Method(Type *type, FunctionPrototype *prototype, Statements *statements) : Function(prototype, statements), mType(type) {}

		Type *getType() const
		{
			return mType;
		}

		virtual void setType(Type *type);

		virtual void childAccept(operation::Operation *operation)
		{
			Function::childAccept(operation);

			if(mType)
			{
				mType->accept(operation);
			}
		}

		//virtual void checkNamedNode(const std::string &name, Node *node);
		//virtual Node *findNamedNode(Identifier *identifier);

#ifdef DEBUG
		virtual void printNode() { LOG("Method"); }
#endif

	private:
		Type *mType;
	};
}

#endif

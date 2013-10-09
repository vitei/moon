#ifndef COMPILER_TREE_STATEMENT_H
#define COMPILER_TREE_STATEMENT_H

#include "common.h"

#include <list>
#include <string>
#include "node.h"
#include "expression.h"
#include "state.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Statement : public Node
	{
	protected:
		Statement() { /* Abstract class */ }
	};

	typedef std::list<Statement *> Statements;

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class Execute final : public Statement
	{
	public:
		Execute(Expression *expression) : mExpression(expression) {}

		Expression *getExpression() const
		{
			return mExpression;
		}

		void setExpression(Expression *expression)
		{
			mExpression = expression;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mExpression)
			{
				mExpression->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Execute"); }
#endif

	private:
		Expression *mExpression;
	};

	class Return final : public Statement
	{
	public:
		Return(Expression *expression) : mReturn(expression) {}

		Expression *getReturn() const
		{
			return mReturn;
		}

		void setReturn(Expression *expression)
		{
			mReturn = expression;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mReturn)
			{
				mReturn->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Return"); }
#endif

	private:
		Expression *mReturn;
	};

//	class SetState final : public Statement
//	{
//	public:
//		SetState(State *state) : mState(state) {}
//
//		State *getState()
//		{
//			return mState;
//		}
//
//		void setState(State *state)
//		{
//			mState = state;
//		}
//
//		virtual void childAccept(operation::Operation *operation)
//		{
//			Statement::childAccept(operation);
//
//			if(mState)
//			{
//				mState->accept(operation);
//			}
//		}
//
//#ifdef DEBUG
//		virtual void printNode() { LOG("SetState"); }
//#endif
//
//	private:
//		State *mState;
//	};

	class TypeDefinition final : public Statement
	{
	public:
		TypeDefinition(std::string name, Type *type) : mName(name), mType(type) {}

		const std::string &getName() const
		{
			return mName;
		}

		void setName(std::string name)
		{
			mName = name;
		}

		Type *getType() const
		{
			return mType;
		}

		void setType(Type *type)
		{
			mType = type;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mType)
			{
				mType->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("TypeDefinition"); }
#endif

	private:
		std::string mName;
		Type *mType;
	};
}

#endif

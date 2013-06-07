#ifndef COMPILER_TREE_STATEMENT_H
#define COMPILER_TREE_STATEMENT_H

#include <list>
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

	class Execute : public Statement
	{
	public:
		Execute(Expression *expression) : mExpression(expression) {}

		Expression *getExpression()
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Execute"); }
#endif

	private:
		Expression *mExpression;
	};

	class Return : public Statement
	{
	public:
		Return(Expression *expression) : mReturn(expression) {}

		Expression *getReturn()
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

#ifdef DEBUG
		virtual void printNode() { LOG("Return"); }
#endif

	private:
		Expression *mReturn;
	};

//	class SetState : public Statement
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
//		virtual void accept(operation::Operation *operation)
//		{
//			setup(operation);
//			childAccept(operation);
//			visit(operation);
//		}
//
//#ifdef DEBUG
//		virtual void printNode() { LOG("SetState"); }
//#endif
//
//	private:
//		State *mState;
//	};
}

#endif

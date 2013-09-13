#ifndef COMPILER_TREE_IF_H
#define COMPILER_TREE_IF_H

#include "common.h"

#include "expression.h"
#include "statement.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class If : public Statement
	{
	public:
		If(Expression *test, Statement *trueStatement, Statement *falseStatement = NULL) : mTest(test), mTrueStatement(trueStatement), mFalseStatement(falseStatement) {}

		Expression *getTest() const
		{
			return mTest;
		}

		void setTest(Expression *test)
		{
			mTest = test;
		}

		Statement *getTrueStatement() const
		{
			return mTrueStatement;
		}

		void setTrueStatement(Statement *trueStatement)
		{
			mTrueStatement = trueStatement;
		}

		Statement *getFalseStatement() const
		{
			return mFalseStatement;
		}

		void setFalseStatement(Statement *falseStatement)
		{
			mFalseStatement = falseStatement;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mTest)
			{
				mTest->accept(operation);
			}

			if(mTrueStatement)
			{
				mTrueStatement->accept(operation);
			}

			if(mFalseStatement)
			{
				mFalseStatement->accept(operation);
			}
		}

#ifdef DEBUG
		virtual void printNode() { LOG("If"); }
#endif

	private:
		Expression *mTest;
		Statement *mTrueStatement;
		Statement *mFalseStatement;
	};
}

#endif

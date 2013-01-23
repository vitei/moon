#ifndef COMPILER_TREE_IF_H
#define COMPILER_TREE_IF_H

#include "node.h"
#include "scope.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class If : public Statement
	{
	public:
		If(Expression *test, Statement *trueStatement, Statement *falseStatement = NULL) : mTest(test), mTrueStatement(trueStatement), mFalseStatement(falseStatement) {}

		Expression *getTest()
		{
			return mTest;
		}

		void setTest(Expression *test)
		{
			mTest = test;
		}

		Statement *getTrueStatement()
		{
			return mTrueStatement;
		}

		void setTrueStatement(Statement *trueScope)
		{
			mTrueStatement = trueScope;
		}

		Statement *getFalseStatement()
		{
			return mFalseStatement;
		}

		void setFalseStatement(Statement *falseScope)
		{
			mFalseStatement = falseScope;
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

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
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

#ifndef COMPILER_TREE_WHILE_H
#define COMPILER_TREE_WHILE_H

#include "node.h"
#include "scope.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class While : public Statement
	{
	public:
		While(Expression *test, Statement *loopStatement) : mTest(test), mLoopStatement(loopStatement) {}

		Expression *getTest()
		{
			return mTest;
		}

		void setTest(Expression *test)
		{
			mTest = test;
		}

		Statement *getLoopStatement()
		{
			return mLoopStatement;
		}

		void setLoopStatement(Statement *loopStatement)
		{
			mLoopStatement = loopStatement;
		}

		virtual void childAccept(operation::Operation *operation)
		{
			Statement::childAccept(operation);

			if(mTest)
			{
				mTest->accept(operation);
			}

			if(mLoopStatement)
			{
				mLoopStatement->accept(operation);
			}
		}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

#ifdef DEBUG
		virtual void printNode() { LOG("While"); }
#endif

	private:
		Expression *mTest;
		Statement *mLoopStatement;
	};
}

#endif
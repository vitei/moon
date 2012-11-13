#ifndef TREE_STATEMENT_H
#define TREE_STATEMENT_H

#include "node.h"
#include "expression.h"


namespace tree
{
	class Statement : public Node
	{
		public:
			Statement() : mSibling(0) {}

			void setSibling(Statement *sibling)
			{
				mSibling = sibling;
			}

		private:
			Statement *mSibling;
	};

	class ExpressionStatement : public Statement
	{
	public:
		ExpressionStatement(Expression *expression) {}
	};
}

#endif

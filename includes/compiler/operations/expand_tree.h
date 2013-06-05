#ifndef COMPILER_OPERATIONS_EXPAND_TREE_H
#define COMPILER_OPERATIONS_EXPAND_TREE_H

#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ExpandTree : public Restructure
	{
	public:
		static void run(tree::Program *program);

		virtual tree::Node *restructure(tree::OperatorAssign *operatorAssign);

	private:
		ExpandTree() {}
	};
}

#endif

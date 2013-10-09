#ifndef COMPILER_OPERATIONS_REMOVE_DEAD_NODES_H
#define COMPILER_OPERATIONS_REMOVE_DEAD_NODES_H

#include "compiler/tree.h"
#include "restructure.h"

namespace operation
{
	class RemoveDeadNodes final : public Restructure
	{
	public:
		static void run(tree::Program *program);

		virtual tree::Node *restructure(tree::Execute *execute);

	private:
		RemoveDeadNodes() {}
	};
}

#endif

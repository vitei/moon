#ifndef COMPILER_OPERATIONS_DEPTH_RESTRUCTURE_H
#define COMPILER_OPERATIONS_DEPTH_RESTRUCTURE_H

#include <queue>
#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class DepthRestructure : public Restructure
	{
	public:
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);

	protected:
		DepthRestructure() {}
	};
}

#endif

#ifndef COMPILER_OPERATIONS_CHECK_SCOPING_H
#define COMPILER_OPERATIONS_CHECK_SCOPING_H

#include "compiler/tree.h"
#include "operation.h"

namespace operation
{
	class CheckScoping final : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void setup(tree::For *forStatement);
		virtual void setup(tree::While *whileStatement);

		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::For *forStatement);
		virtual void visit(tree::While *whileStatement);

		virtual void visit(tree::Break *breakStatement);

	private:
		CheckScoping() : mLoopDepth(0) {}

		unsigned int mLoopDepth;
	};
}

#endif

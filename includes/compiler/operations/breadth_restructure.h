#ifndef COMPILER_OPERATIONS_BREADTH_RESTRUCTURE_H
#define COMPILER_OPERATIONS_BREADTH_RESTRUCTURE_H

#include <queue>
#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class BreadthRestructure : public Restructure
	{
	public:
		void add(tree::Scope *scope);
		void process();

		virtual void beginScope(tree::Scope *scope) {}

		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);

	protected:
		BreadthRestructure() {}

		virtual tree::Scope *getCurrentScope()
		{
			return mVisitNext.front();
		}

		std::queue<tree::Scope *> mVisitNext;
	};
}

#endif

#ifndef COMPILER_OPERATIONS_SCOPE_PARENTS_H
#define COMPILER_OPERATIONS_SCOPE_PARENTS_H

#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class ScopeParents : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::GlobalScoping *globalScoping);
		virtual void visit(tree::SharedScoping *sharedScoping);

	private:
		ScopeParents() : mScope(NULL) {}

		tree::Scope *mScope;
	};
}

#endif

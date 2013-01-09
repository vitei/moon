#ifndef COMPILER_OPERATIONS_SCOPE_PARENTS_H
#define COMPILER_OPERATIONS_SCOPE_PARENTS_H

#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ScopeParents : public Restructure
	{
	public:
		static void run(tree::Program *program);

		virtual void setup(tree::Program *program);
		virtual void setup(tree::Aggregate *aggregate);

		virtual void doRestructure(tree::Function *function);
		virtual void doRestructure(tree::Scope *scope);

		virtual tree::Node *restructure(tree::GlobalScoping *globalScoping);
		virtual tree::Node *restructure(tree::SharedScoping *sharedScoping);

	private:
		ScopeParents() : mProgramScope(NULL), mAggregateScope(NULL) {}

		tree::Program *mProgramScope;
		tree::Aggregate *mAggregateScope;
	};
}

#endif

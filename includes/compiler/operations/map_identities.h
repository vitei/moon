#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include <queue>
#include "compiler/tree.h"
#include "breadth_restructure.h"


namespace operation
{
	class MapIdentities : public BreadthRestructure
	{
	public:
		static void run(tree::Program *program);

		virtual void beginScope(tree::Scope *scope);

		virtual void setup(tree::Function *function);
		virtual void setup(tree::GlobalScoping *globalScoping);
		virtual void setup(tree::SharedScoping *sharedScoping);

		virtual void visit(tree::Function *function);

		virtual tree::Node *restructure(tree::GlobalScoping *globalScoping);
		virtual tree::Node *restructure(tree::SharedScoping *sharedScoping);
		virtual tree::Node *restructure(tree::Identity *identity);

	private:
		MapIdentities() {}

		tree::Program *mProgramScope;
		tree::Aggregate *mAggregateScope;
		tree::Scope *mCurrentScope;
	};
}

#endif

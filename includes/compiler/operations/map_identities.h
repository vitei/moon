#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include <queue>
#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class MapIdentities : public Restructure
	{
	public:
		static void run(tree::Program *program);

		void add(tree::Scope *scope);
		void process();

		virtual void setup(tree::Function *function);
		virtual void setup(tree::GlobalScoping *globalScoping);
		virtual void setup(tree::SharedScoping *sharedScoping);

		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);

		virtual tree::Node *restructure(tree::Scope *scope);
		virtual tree::Node *restructure(tree::GlobalScoping *globalScoping);
		virtual tree::Node *restructure(tree::SharedScoping *sharedScoping);

		tree::Node *restructure(tree::Identity *identity);

	private:
		MapIdentities() {}

		std::queue<tree::Scope *> mVisitNext;

		tree::Scope *mProgramScope;
		tree::Scope *mAggregateScope;
		tree::Scope *mScope;
		tree::Scope *mCurrentScope;
	};
}

#endif

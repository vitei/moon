#ifndef COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H
#define COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H

#include <queue>
#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ResolveIdentities : public Restructure
	{
	public:
		static void run(tree::Program *program);

		void add(tree::Scope *scope);
		void process();

		void dispatch(tree::Scope *scope);

		virtual void visit(tree::Expression *expression);

		virtual void setup(tree::Equals *equals);

		virtual tree::Node *restructure(tree::Identifier *identifier);
		virtual tree::Node *restructure(tree::Execute *execute);

	private:
		ResolveIdentities() {}

		std::queue<tree::Scope *> mVisitNext;
	};
}

#endif

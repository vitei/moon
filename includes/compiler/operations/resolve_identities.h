#ifndef COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H
#define COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H

#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ResolveIdentities : public Restructure
	{
	public:
		static void run(tree::Program *program);

		virtual void visit(tree::Expression *expression);

		virtual tree::Node *restructure(tree::Identifier *identifier);

	private:
		ResolveIdentities() {}
	};
}

#endif

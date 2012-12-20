#ifndef COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H
#define COMPILER_OPERATIONS_RESOLVE_IDENTITIES_H

#include "compiler/tree.h"
#include "depth_restructure.h"


namespace operation
{
	class ResolveIdentities : public DepthRestructure
	{
	public:
		static void run(tree::Program *program);

		virtual tree::Node *restructure(tree::Identifier *identifier);

	private:
		ResolveIdentities() {}
	};
}

#endif

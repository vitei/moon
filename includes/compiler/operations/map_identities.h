#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class MapIdentities : public Restructure
	{
	public:
		static void run(tree::Program *program);

		/*virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Identity *identity);*/

	private:
		MapIdentities() {}
	};
}

#endif

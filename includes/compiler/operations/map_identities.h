#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class MapIdentities : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void process(tree::Program *program);
		virtual void process(tree::Scope *scope);
		virtual void process(tree::FunctionPrototype *functionPrototype);
		virtual void process(tree::Variable *variable);

	private:
		MapIdentities() {}
	};
}

#endif

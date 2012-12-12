#ifndef COMPILER_OPERATIONS_MAP_IDENTITIES_H
#define COMPILER_OPERATIONS_MAP_IDENTITIES_H

#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class MapIdentities : public Operation
	{
	public:
		MapIdentities(tree::Program *program) : mProgram(program) {}

		virtual void run();

	private:
		tree::Program *mProgram;
	};
}

#endif

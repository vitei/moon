#ifndef COMPILER_GENERATORS_GENERATOR_H
#define COMPILER_GENERATORS_GENERATOR_H

#include "compiler/tree.h"
#include "compiler/operations/operation.h"


namespace generator
{
	class Generator
	{
	public:
		virtual void run(tree::Program *program) = 0;

	protected:
		Generator() { /* Abstract class */ }
	};
}

#endif

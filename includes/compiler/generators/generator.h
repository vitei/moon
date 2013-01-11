#ifndef COMPILER_GENERATORS_GENERATOR_H
#define COMPILER_GENERATORS_GENERATOR_H

#include <ostream>
#include "compiler/tree.h"
#include "compiler/operations/operation.h"


namespace generator
{
	class Generator
	{
	public:
		virtual void run(std::ostream &output, tree::Program *program) = 0;

	protected:
		Generator() { /* Abstract class */ }
	};
}

#endif

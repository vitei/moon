#include <sstream> // DEBUG


#ifndef COMPILER_GENERATORS_C_H
#define COMPILER_GENERATORS_C_H

#include "compiler/tree.h"
#include "generator.h"


namespace generator
{
	class C : public Generator
	{
	public:
		C() {}

		virtual void run(tree::Program *program);

		virtual void visit(tree::Program *program);

		void outputDeclaration(tree::Identity *identity);

	private:
		std::stringstream mOutput; // DEBUG
	};
}

#endif

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

		virtual void dispatch(tree::Node *node);

		virtual void generate(tree::Node *node) { /*ERROR("Should never reach here");*/ }
		virtual void generate(tree::Scope *scope);
		virtual void generate(tree::Program *program);
		virtual void generate(tree::Function *function);

		void outputDeclaration(tree::Identity *identity);

	private:
		std::stringstream mOutput; // DEBUG
	};
}

#endif

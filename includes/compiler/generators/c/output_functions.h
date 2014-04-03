#ifndef COMPILER_GENERATORS_C_OUTPUT_FUNCTIONS_H
#define COMPILER_GENERATORS_C_OUTPUT_FUNCTIONS_H

#include "common.h"

#include "compiler/operations.h"
#include "compiler/tree.h"
#include "printer.h"

namespace generator
{
	namespace c
	{
		class OutputFunctions : public operation::Operation
		{
		public:
			static void run(generator::c::Printer *printer, tree::Program *program);

			virtual void visit(tree::Scope *scope);
			virtual void visit(tree::Function *function);
			virtual void visit(tree::Method *method);

		private:
			OutputFunctions() {}

			generator::c::Printer *mPrinter;
		};
	}
}

#endif

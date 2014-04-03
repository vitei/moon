#ifndef COMPILER_GENERATORS_C_OUTPUT_TYPES_H
#define COMPILER_GENERATORS_C_OUTPUT_TYPES_H

#include "common.h"

#include <set>
#include "compiler/operations.h"
#include "compiler/tree.h"
#include "printer.h"

namespace generator
{
	namespace c
	{
		class OutputTypes : public operation::Operation
		{
		public:
			static void run(generator::c::Printer *printer, tree::Program *program);

			virtual void visit(tree::Scope *scope);
			virtual void visit(tree::UDT *udt);

		private:
			OutputTypes() {}

			generator::c::Printer *mPrinter;
			std::set<tree::UDT *>mTypesOutput;
		};
	}
}

#endif

#ifndef COMPILER_GENERATORS_OBJ_C_OUTPUT_TYPES_H
#define COMPILER_GENERATORS_OBJ_C_OUTPUT_TYPES_H

#include "common.h"

#include <set>
#include "compiler/operations.h"
#include "compiler/tree.h"
#include "printer.h"

namespace generator
{
	namespace objC
	{
		class OutputTypes : public operation::Operation
		{
		public:
			static void run(generator::objC::Printer *printer, tree::Program *program);

			virtual void visit(tree::Scope *scope);
			virtual void visit(tree::UDT *udt);

		private:
			OutputTypes() {}

			generator::objC::Printer *mPrinter;
			std::set<tree::UDT *>mTypesOutput;
		};
	}
}

#endif

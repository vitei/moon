#ifndef COMPILER_GENERATORS_OBJ_C_OUTPUT_VARIABLES_H
#define COMPILER_GENERATORS_OBJ_C_OUTPUT_VARIABLES_H

#include "common.h"

#include <list>
#include "compiler/operations.h"
#include "compiler/tree.h"
#include "printer.h"

namespace generator
{
	namespace objC
	{
		class OutputVariables : public operation::Operation
		{
		public:
			static void run(generator::objC::Printer *printer, tree::Program *program);

			virtual void visit(tree::Program *program);
			virtual void visit(tree::Aggregate *aggregate);
			virtual void visit(tree::Use *use);

		private:
			OutputVariables() {}

			void visitScope(tree::Scope *scope);

			std::list<tree::TypedIdentity *> mIdentities;
			generator::objC::Printer *mPrinter;
		};
	}
}

#endif

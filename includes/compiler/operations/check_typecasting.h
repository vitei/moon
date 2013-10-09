#ifndef COMPILER_OPERATIONS_CHECK_TYPECASTING_H
#define COMPILER_OPERATIONS_CHECK_TYPECASTING_H

#include "compiler/tree.h"
#include "operation.h"

namespace operation
{
	class CheckTypecasting final : public Operation
	{
	public:
		static bool run(tree::Program *program);

		virtual void visit(tree::Cast *cast);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Scope *scope);

	private:
		CheckTypecasting() {}
	};
}

#endif

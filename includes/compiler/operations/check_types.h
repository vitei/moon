#ifndef COMPILER_OPERATIONS_CHECK_TYPES_H
#define COMPILER_OPERATIONS_CHECK_TYPES_H

#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class CheckTypes : public Operation
	{
	public:
		static bool run(tree::Program *program);

		virtual void visit(tree::TypedIdentity *typedIdentity);
		virtual void visit(tree::Constant *constant);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);

	private:
		CheckTypes() {}
	};
}

#endif

#ifndef COMPILER_OPERATIONS_INFER_TYPES_H
#define COMPILER_OPERATIONS_INFER_TYPES_H

#include <map>
#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class InferTypes : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void visit(tree::Access *access);
		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Return *returnStatement);

	private:
		InferTypes() : mPrototype(NULL) {}

		tree::FunctionPrototype *mPrototype;
		std::map<tree::TypedIdentity *, tree::Type *> mTypeResolution;
	};
}

#endif

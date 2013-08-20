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
		virtual void visit(tree::Identity *identity);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Return *returnStatement);

		virtual bool doProcessChildren(tree::Assign *assign) { return false; }

	private:
		InferTypes() : mPrototype(NULL), mAssignIdentity(NULL), mSelfReference(false) {}

		tree::FunctionPrototype *mPrototype;
		std::map<tree::TypedIdentity *, tree::Type *> mTypeResolution;

		tree::TypedIdentity *mAssignIdentity;
		bool mSelfReference;
	};
}

#endif

#ifndef COMPILER_OPERATIONS_INFER_TYPES_H
#define COMPILER_OPERATIONS_INFER_TYPES_H

#include <map>
#include <set>
#include "compiler/tree.h"
#include "operation.h"

namespace operation
{
	class InferTypes final : public Operation
	{
	public:
		static bool run(tree::Program *program);

		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::For *forStatement);
		virtual void visit(tree::Identity *identity);
		virtual void visit(tree::Identifier *identifier);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Return *returnStatement);

		virtual bool doProcessChildren(tree::Assign *assign) { return false; }

	private:
		InferTypes() : mValidated(false), mPrototype(nullptr), mAssignIdentity(nullptr), mSelfReference(false) {}

		bool mValidated;

		tree::FunctionPrototype *mPrototype;
		std::map<tree::TypedIdentity *, tree::Type *> mTypeResolution;

		tree::TypedIdentity *mAssignIdentity;
		bool mSelfReference;

		std::set<tree::Identity *> mAccessedUnresolvedIdentities;
	};
}

#endif

#ifndef COMPILER_OPERATIONS_RESOLVE_TYPES_H
#define COMPILER_OPERATIONS_RESOLVE_TYPES_H

#include "compiler/tree.h"
#include "operation.h"

namespace operation
{
	class ResolveTypes final : public Operation
	{
	public:
		static bool run(tree::Program *program);

		virtual void visit(tree::Access *access);
		virtual void visit(tree::ArrayAccess *arrayAccess);
		virtual void visit(tree::Operation *operation);
		virtual void visit(tree::BooleanBinaryOperation *booleanBinaryOperation);
		virtual void visit(tree::BooleanUnaryOperation *booleanUnaryOperation);
		virtual void visit(tree::ComputedArray *computedArray);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::IfExpression *ifExpression);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::This *th1s);

	private:
		ResolveTypes() : mValidated(false), mCurrentScope(nullptr) {}

		bool mValidated;
		tree::Scope *mCurrentScope;
	};
}

#endif

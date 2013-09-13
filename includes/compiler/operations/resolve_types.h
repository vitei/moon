#ifndef COMPILER_OPERATIONS_RESOLVE_TYPES_H
#define COMPILER_OPERATIONS_RESOLVE_TYPES_H

#include "compiler/tree.h"
#include "operation.h"

namespace operation
{
	class ResolveTypes : public Operation
	{
	public:
		static bool run(tree::Program *program);

		virtual void visit(tree::Access *access);
		virtual void visit(tree::ArrayAccess *arrayAccess);
		virtual void visit(tree::BinaryOperation *binaryOperation);
		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::BooleanBinaryOperation *booleanBinaryOperation);
		virtual void visit(tree::UnaryOperation *unaryOperation);
		virtual void visit(tree::BooleanUnaryOperation *booleanUnaryOperation);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::IfExpression *ifExpression);
		virtual void visit(tree::Scope *scope);

		void setOperationType(tree::Operation *operation, tree::Type *type);

	private:
		ResolveTypes() : mValidated(false) {}

		bool mValidated;
	};
}

#endif

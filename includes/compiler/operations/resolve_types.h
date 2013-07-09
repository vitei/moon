#ifndef COMPILER_OPERATIONS_RESOLVE_TYPES_H
#define COMPILER_OPERATIONS_RESOLVE_TYPES_H

#include <map>
#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class ResolveTypes : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void visit(tree::Access *access);
		virtual void visit(tree::ArrayAccess *arrayAccess);
		virtual void visit(tree::BinaryOperation *binaryOperation);
		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::BooleanBinaryOperation *booleanBinaryOperation);
		virtual void visit(tree::UnaryOperation *unaryOperation);
		virtual void visit(tree::BooleanUnaryOperation *booleanUnaryOperation);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Return *returnStatement);

		bool resolve();

		void setOperationType(tree::Operation *operation, tree::Type *type);

	private:
		ResolveTypes(): mPrototype(NULL) {}

		tree::FunctionPrototype *mPrototype;
		std::map<tree::Expression *, tree::Type *> mTypeResolution;
	};
}

#endif

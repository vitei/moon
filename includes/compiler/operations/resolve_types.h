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
		virtual void visit(tree::BinaryExpression *binaryExpression);
		virtual void visit(tree::Assign *assign);
		virtual void visit(tree::BooleanBinaryExpression *booleanBinaryExpression);
		virtual void visit(tree::UnaryExpression *unaryExpression);
		virtual void visit(tree::BooleanUnaryExpression *booleanUnaryExpression);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::Return *returnStatement);

		bool resolve();

	private:
		ResolveTypes(): mPrototype(NULL) {}

		tree::FunctionPrototype *mPrototype;
		std::map<tree::Expression *, tree::Type *> mTypeResolution;
	};
}

#endif

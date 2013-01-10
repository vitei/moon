#ifndef COMPILER_OPERATIONS_TYPE_EXPRESSIONS_H
#define COMPILER_OPERATIONS_TYPE_EXPRESSIONS_H

#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class TypeExpressions : public Operation
	{
	public:
		static void run(tree::Program *program);

		virtual void visit(tree::Access *access);
		virtual void visit(tree::ArrayAccess *arrayAccess);
		virtual void visit(tree::BinaryExpression *binaryExpression);
		virtual void visit(tree::UnaryExpression *unaryExpression);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Scope *scope);

	private:
		TypeExpressions() {}
	};
}

#endif
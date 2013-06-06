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

		virtual void visit(tree::BinaryExpression *binaryExpression);
		virtual void visit(tree::BooleanBinaryExpression *booleanBinaryExpression);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::Scope *scope);
		virtual void visit(tree::Function *function);
		virtual void visit(tree::If *ifStatement);
		virtual void visit(tree::While *whileStatement);
		virtual void visit(tree::Return *returnStatement);

		tree::Cast *createCast(tree::Type *type, tree::Expression *expression);

	private:
		TypeExpressions() : mPrototype(NULL) {}

		tree::FunctionPrototype *mPrototype;
	};
}

#endif

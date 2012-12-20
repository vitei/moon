#ifndef COMPILER_OPERATIONS_RESTRUCTURE_H
#define COMPILER_OPERATIONS_RESTRUCTURE_H

#include <stack>
#include "compiler/tree.h"
#include "operation.h"


namespace operation
{
	class Restructure : public Operation
	{
	public:
		virtual void visit(tree::Node *Node);
		virtual void visit(tree::Identity *identity);
		virtual void visit(tree::TypedIdentity *typedIdentity);
		virtual void visit(tree::Access *access);
		virtual void visit(tree::Literal *literal);
		virtual void visit(tree::UnaryExpression *unaryExpression);
		virtual void visit(tree::BinaryExpression *binaryExpression);
		virtual void visit(tree::Cast *cast);
		virtual void visit(tree::FunctionCall *functionCall);
		virtual void visit(tree::FunctionPrototype *functionPrototype);
		virtual void visit(tree::Scoping *scoping);
		virtual void visit(tree::Execute *execute);
		virtual void visit(tree::Return *opReturn);
		virtual void visit(tree::SetState *setState);

		virtual void visit(tree::Function *function) = 0;
		virtual void visit(tree::Scope *scope) = 0;

		virtual tree::Node *restructure(tree::Node *node) { return node; }
		PROCESS_ACTIONS(tree::Node *, restructure)

	protected:
		Restructure() { /* Abstract class */ }

		virtual tree::Scope *getCurrentScope() = 0;

		std::stack<tree::Node *> mNodeMap;
	};
}

#endif

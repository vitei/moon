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
		virtual void visit(tree::Node *node);

		virtual void dispatch(tree::Node *node);
		virtual void dispatch(tree::Expression *expression);
		virtual void dispatch(tree::Access *access);
		virtual void dispatch(tree::UnaryExpression *unaryExpression);
		virtual void dispatch(tree::BinaryExpression *binaryExpression);
		virtual void dispatch(tree::Cast *cast);
		virtual void dispatch(tree::FunctionCall *functionCall);
		virtual void dispatch(tree::Scoping *scoping);
		virtual void dispatch(tree::Execute *execute);
		virtual void dispatch(tree::Return *opReturn);
		virtual void dispatch(tree::SetState *setState);
		virtual void dispatch(tree::FunctionPrototype *functionPrototype);
		virtual void dispatch(tree::Function *function);
		virtual void dispatch(tree::Scope *scope);

		virtual tree::Node *restructure(tree::Node *node) { return node; }
		PROCESS_ACTIONS(tree::Node *, restructure)

	protected:
		Restructure() : mCurrentScope(NULL) { /* Abstract class */ }

		tree::Scope *getCurrentScope()
		{
			return mCurrentScope;
		}

		std::stack<tree::Node *> mNodeMap;

	private:
		tree::Scope *mCurrentScope;
	};
}

#endif

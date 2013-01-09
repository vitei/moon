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

		virtual void doRestructure(tree::Node *node);
		virtual void doRestructure(tree::Expression *expression);
		virtual void doRestructure(tree::Access *access);
		virtual void doRestructure(tree::UnaryExpression *unaryExpression);
		virtual void doRestructure(tree::BinaryExpression *binaryExpression);
		virtual void doRestructure(tree::Cast *cast);
		virtual void doRestructure(tree::FunctionCall *functionCall);
		virtual void doRestructure(tree::Scoping *scoping);
		virtual void doRestructure(tree::Execute *execute);
		virtual void doRestructure(tree::Return *opReturn);
		virtual void doRestructure(tree::SetState *setState);
		virtual void doRestructure(tree::FunctionPrototype *functionPrototype);
		virtual void doRestructure(tree::Function *function);
		virtual void doRestructure(tree::Scope *scope);

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

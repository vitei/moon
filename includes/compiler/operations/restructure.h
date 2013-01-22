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

		void dispatch(tree::Node *node);
		void dispatch(tree::Expression *expression);
		void dispatch(tree::Access *access);
		void dispatch(tree::UnaryExpression *unaryExpression);
		void dispatch(tree::BinaryExpression *binaryExpression);
		void dispatch(tree::Cast *cast);
		void dispatch(tree::Import *import);
		void dispatch(tree::FunctionCall *functionCall);
		void dispatch(tree::Scoping *scoping);
		void dispatch(tree::Execute *execute);
		void dispatch(tree::Return *opReturn);
		void dispatch(tree::SetState *setState);
		void dispatch(tree::FunctionPrototype *functionPrototype);
		void dispatch(tree::Function *function);
		void dispatch(tree::Scope *scope);

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

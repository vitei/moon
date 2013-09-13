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
		virtual void dispatch(tree::BinaryOperation *binaryOperation);
		virtual void dispatch(tree::UnaryOperation *unaryOperation);
		virtual void dispatch(tree::Scope *scope);
		virtual void dispatch(tree::Scoping *scoping);
		virtual void dispatch(tree::SizedType *sizedType);
		virtual void dispatch(tree::Assign *assign);
		virtual void dispatch(tree::Cast *cast);
		virtual void dispatch(tree::FunctionCall *functionCall);
		virtual void dispatch(tree::IfExpression *ifExpression);
		virtual void dispatch(tree::Function *function);
		virtual void dispatch(tree::If *ifStatement);
		virtual void dispatch(tree::Import *import);
		virtual void dispatch(tree::Execute *execute);
		virtual void dispatch(tree::Return *opReturn);
		virtual void dispatch(tree::TypeDefinition *typeDefinition);
		virtual void dispatch(tree::While *whileStatement);
		virtual void dispatch(tree::UDT *udt);
		virtual void dispatch(tree::Array *array);

		virtual tree::Node *restructure(tree::Node *node) { return node; }
		PROCESS_ACTIONS(tree::Node *, restructure)

	protected:
		Restructure() : mCurrentScope(NULL) { /* Abstract class */ }

		void processFunctionParameters(tree::Function *function);

		tree::Scope *getCurrentScope()
		{
			return mCurrentScope;
		}

		std::stack<tree::Node *> mNodeMap;
		tree::Scope *mCurrentScope;
	};

	#define RESTRUCTURE_GET(dst, type, source) \
		if(source) \
		{ \
			dst = static_cast<type *>(mNodeMap.top()); \
			mNodeMap.pop(); \
		}
}

#endif

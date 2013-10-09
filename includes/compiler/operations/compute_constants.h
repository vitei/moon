#ifndef COMPILER_OPERATIONS_COMPUTE_CONSTANTS_H
#define COMPILER_OPERATIONS_COMPUTE_CONSTANTS_H

#include "compiler/tree.h"
#include "restructure.h"

namespace operation
{
	class ComputeConstants final : public Restructure
	{
	public:
		static bool run(tree::Program *program);

		virtual tree::Node *restructure(tree::Assign *assign);
		virtual tree::Node *restructure(tree::Cast *cast);
		virtual tree::Node *restructure(tree::IfExpression *ifExpression);
		virtual tree::Node *restructure(tree::Constant *constant);
		virtual tree::Node *restructure(tree::BinaryOperation *binaryOperation);
		virtual tree::Node *restructure(tree::UnaryOperation *unaryOperation);

	private:
		ComputeConstants() : mValidated(false) {}

		bool mValidated;
	};
}

#endif

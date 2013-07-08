#ifndef COMPILER_OPERATIONS_COMPUTE_CONSTANTS_H
#define COMPILER_OPERATIONS_COMPUTE_CONSTANTS_H

#include "compiler/tree.h"
#include "restructure.h"


namespace operation
{
	class ComputeConstants : public Restructure
	{
	public:
		static void run(tree::Program *program);

		virtual tree::Node *restructure(tree::Cast *cast);

	private:
		ComputeConstants() {}
	};
}

#endif
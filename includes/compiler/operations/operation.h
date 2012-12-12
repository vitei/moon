#ifndef COMPILER_OPERATIONS_OPERATION_H
#define COMPILER_OPERATIONS_OPERATION_H

#include "operation.h"


namespace operation
{
	class Operation
	{
	public:
		virtual void run() = 0;
	};
}

#endif

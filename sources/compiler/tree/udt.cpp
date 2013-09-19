#include "common.h"

#include "compiler/tree/udt.h"

bool tree::UDT::canPerform(const Operation &operation) const
{
	if(dynamic_cast<const tree::Assign *>(&operation))
	{
		return true;
	}

	return false;
}

bool tree::UDT::equals(const tree::Type &type) const
{
	// Super easy...
	return &type == this;
}

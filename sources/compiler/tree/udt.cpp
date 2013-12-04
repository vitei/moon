#include "common.h"

#include "compiler/tree/udt.h"

bool tree::UDT::equals(const tree::Type &type) const
{
	// Super easy...
	return &type == this;
}

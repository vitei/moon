#include "common.h"

#include <string>
#include "compiler/tree/method.h"

void tree::Method::setType(tree::Type *type)
{
	tree::Type *t;

	if(type == NULL)
	{
		mType = NULL;
	}
	else if(tree::node_cast<tree::Identifier *>(type) ||
	        tree::node_cast<tree::Type *>(type))
	{
		mType = type;
	}
	else
	{
		throw tree::Method::InvalidTypeException(this);
	}
}

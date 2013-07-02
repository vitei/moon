#include "compiler/tree.h"


bool tree::Int::canCast(const tree::Type &from) const
{
	if(dynamic_cast<const tree::Bool *>(&from))
	{
		return true;
	}

	return false;
}

bool tree::Float::canCast(const tree::Type &from) const
{
	if(dynamic_cast<const tree::Bool *>(&from) ||
	   dynamic_cast<const tree::Int *>(&from))
	{
		return true;
	}

	return false;
}

bool tree::String::canCast(const tree::Type &from) const
{
	const tree::String *string;

	if((string = dynamic_cast<const tree::String *>(&from)))
	{
		if(string->getSize() <= getSize())
		{
			return true;
		}
	}

	return false;
}

bool tree::Array::canCast(const tree::Type &from) const
{
	/*if(dynamic_cast<tree::Bool *>(&from) ||
	   dynamic_cast<tree::Int *>(&from))
	{
		return true;
	}*/

	return false;
}




#include "compiler/tree.h"


bool tree::Bool::canCast(const tree::Type &from, bool autoCast) const
{
	if(!autoCast)
	{
		if(dynamic_cast<const tree::Int *>(&from) ||
		   dynamic_cast<const tree::Float *>(&from) ||
		   dynamic_cast<const tree::String *>(&from))
		{
			return true;
		}
	}

	return false;
}

bool tree::Bool::canPerform(const Operation &operation) const
{
	if(dynamic_cast<const tree::Assign *>(&operation) ||
	   dynamic_cast<const tree::BooleanBinaryOperation *>(&operation) ||
	   dynamic_cast<const tree::BooleanUnaryOperation *>(&operation))
	{
		return true;
	}

	return false;
}

bool tree::Int::canCast(const tree::Type &from, bool autoCast) const
{
	if(dynamic_cast<const tree::Bool *>(&from))
	{
		return true;
	}
	else if(!autoCast)
	{
		if(dynamic_cast<const tree::Float *>(&from))
		{
			return true;
		}
	}

	return false;
}

bool tree::Int::canPerform(const Operation &operation) const
{
	if(dynamic_cast<const tree::Assign *>(&operation) ||
	   dynamic_cast<const tree::Or *>(&operation) ||
	   dynamic_cast<const tree::Xor *>(&operation) ||
	   dynamic_cast<const tree::And *>(&operation) ||
	   dynamic_cast<const tree::Add *>(&operation) ||
	   dynamic_cast<const tree::Subtract *>(&operation) ||
	   dynamic_cast<const tree::Multiply *>(&operation) ||
	   dynamic_cast<const tree::Divide *>(&operation) ||
	   dynamic_cast<const tree::Modulus *>(&operation) ||
	   dynamic_cast<const tree::Not *>(&operation) ||
	   dynamic_cast<const tree::Minus *>(&operation))
	{
		return true;
	}

	return false;
}

bool tree::Float::canCast(const tree::Type &from, bool autoCast) const
{
	if(dynamic_cast<const tree::Bool *>(&from) ||
	   dynamic_cast<const tree::Int *>(&from))
	{
		return true;
	}

	return false;
}

bool tree::Float::canPerform(const Operation &operation) const
{
	if(dynamic_cast<const tree::Assign *>(&operation) ||
	   dynamic_cast<const tree::Add *>(&operation) ||
	   dynamic_cast<const tree::Subtract *>(&operation) ||
	   dynamic_cast<const tree::Multiply *>(&operation) ||
	   dynamic_cast<const tree::Divide *>(&operation) ||
	   dynamic_cast<const tree::Minus *>(&operation))
	{
		return true;
	}

	return false;
}

bool tree::String::canCast(const tree::Type &from, bool autoCast) const
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

tree::Array::Array(Type *type, long long size) : mType(type), mSize(size)
{
	if(mSize <= 0)
	{
		throw tree::Array::InvalidSizeException(this);
	}
}

bool tree::Array::canCast(const tree::Type &from, bool autoCast) const
{
	/*if(dynamic_cast<tree::Bool *>(&from) ||
	   dynamic_cast<tree::Int *>(&from))
	{
		return true;
	}*/

	return false;
}




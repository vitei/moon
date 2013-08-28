#include "compiler/tree.h"


bool tree::SizedType::isResolved() const
{
	return !mSize || dynamic_cast<tree::IntLiteral *>(mSize);
}

void tree::SizedType::childAccept(operation::Operation *operation)
{
	Type::childAccept(operation);

	if(mSize)
	{
		mSize->accept(operation);
	}
}

bool tree::Void::equals(const Type &type) const
{
	return dynamic_cast<const Void *>(&type) != 0;
}

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

bool tree::Bool::equals(const Type &type) const
{
	return dynamic_cast<const Bool *>(&type) != 0;
}

bool tree::Int::canCast(const tree::Type &from, bool autoCast) const
{
	if(dynamic_cast<const tree::Bool *>(&from) ||
	   dynamic_cast<const tree::Int *>(&from)) // FIXME, needs size
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

bool tree::Int::equals(const Type &type) const
{
	const Int *integer = dynamic_cast<const Int *>(&type);

	if(integer)
	{
		return mSize == integer->mSize; // FIXME
	}
	else
	{
		return false;
	}
}

bool tree::Float::canCast(const tree::Type &from, bool autoCast) const
{
	if(dynamic_cast<const tree::Bool *>(&from) ||
	   dynamic_cast<const tree::Int *>(&from) ||
	   dynamic_cast<const tree::Float *>(&from)) // FIXME, needs size
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

bool tree::Float::equals(const Type &type) const
{
	const Float *floatingPoint = dynamic_cast<const Float *>(&type);

	if(floatingPoint)
	{
		return mSize == floatingPoint->mSize; // FIXME
	}
	else
	{
		return false;
	}
}

bool tree::String::canCast(const tree::Type &from, bool autoCast) const
{
	const tree::String *string;

	if((string = dynamic_cast<const tree::String *>(&from))/* && string->getSize() <= getSize()*/) // FIXME
	{
		return true;
	}

	return false;
}

bool tree::String::equals(const Type &type) const
{
	const String *string = dynamic_cast<const String *>(&type);

	if(string)
	{
		tree::Literal *aSize = dynamic_cast<tree::Literal *>(mSize);
		tree::Literal *bSize = dynamic_cast<tree::Literal *>(string->mSize);

		return aSize && bSize && *aSize == *bSize;
	}
	else
	{
		return false;
	}
}

/*tree::Array::Array(Type *type) : mType(type)
{
}*/

bool tree::Array::canCast(const tree::Type &from, bool autoCast) const
{
	/*if(dynamic_cast<tree::Bool *>(&from) ||
	   dynamic_cast<tree::Int *>(&from))
	{
		return true;
	}*/

	return false;
}

bool tree::Array::equals(const Type &type) const
{
	const Array *array = dynamic_cast<const Array *>(&type);

	if(array)
	{
		tree::Literal *aSize = dynamic_cast<tree::Literal *>(mSize);
		tree::Literal *bSize = dynamic_cast<tree::Literal *>(array->mSize);

		return aSize && bSize && *aSize == *bSize && (*mType == *array->mType);
	}
	else
	{
		return false;
	}
}

void tree::Array::childAccept(operation::Operation *operation)
{
	SizedType::childAccept(operation);

	if(mType)
	{
		mType->accept(operation);
	}
}

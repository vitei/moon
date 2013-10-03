#include "common.h"

#include "compiler/tree/expression.h"
#include "compiler/tree/identifier.h"
#include "compiler/tree/type.h"

void tree::Type::checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node)
{
	const behaviour::NamedMap::NamedNodes  &namedNodes = mAssociatedNamedNodes[association];
	const behaviour::NamedMap::NamedNodes::const_iterator previousValue = namedNodes.find(name);

	if(previousValue != namedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
}

tree::Node *tree::Type::findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier)
{
	const behaviour::NamedMap::NamedNodes &namedNodes = mAssociatedNamedNodes[association];
	const std::string &name = identifier->getName();
	const behaviour::NamedMap::NamedNodes::const_iterator identity = namedNodes.find(name);

	if(identity != namedNodes.end())
	{
		return identity->second;
	}
	else
	{
		throw behaviour::NamedMap::NotFoundException(identifier);
	}
}

void tree::Type::mapAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node)
{
	try
	{
		checkAssociatedNamedNode(association, name, node);
		mAssociatedNamedNodes[association][name] = node;
	}
	catch(behaviour::NamedMap::ExistsException &e)
	{
		// Only process this exception if we're trying to make something new...
		// // FIXME, this is rather nasty
		if(e.node != e.conflict)
		{
			throw e;
		}
	}
}

void tree::SizedType::setSize(tree::Expression *size)
{
	// Check that if the size expression is a literal that it is an integer
	if(!(   dynamic_cast<tree::Literal *>(size)
	     && !dynamic_cast<tree::IntLiteral *>(size)))
	{
		mSize.expression = size;
	}
	else
	{
		throw tree::SizedType::InvalidException(this);
	}
}

bool tree::SizedType::isResolved() const
{
	return !mSize.expression || dynamic_cast<tree::IntLiteral *>(mSize.expression);
}

void tree::SizedType::childAccept(operation::Operation *operation)
{
	Type::childAccept(operation);

	if(mSize.expression)
	{
		mSize.expression->accept(operation);
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
		if(   dynamic_cast<const tree::Int *>(&from)
		   || dynamic_cast<const tree::Float *>(&from)
		   || dynamic_cast<const tree::String *>(&from))
		{
			return true;
		}
	}

	return false;
}

bool tree::Bool::canPerform(const Operation &operation) const
{
	if(   dynamic_cast<const tree::Assign *>(&operation)
	   || dynamic_cast<const tree::BooleanBinaryOperation *>(&operation)
	   || dynamic_cast<const tree::BooleanUnaryOperation *>(&operation))
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
	const tree::Int *fromInt;

	if(   dynamic_cast<const tree::Bool *>(&from)
	   || ((   fromInt = dynamic_cast<const tree::Int *>(&from))
	        && fromInt->getSizeInt() <= getSizeInt()))
	{
		return true;
	}
	else if(!autoCast)
	{
		if(   dynamic_cast<const tree::Int *>(&from)
		   || dynamic_cast<const tree::Float *>(&from))
		{
			return true;
		}
	}

	return false;
}

bool tree::Int::canPerform(const Operation &operation) const
{
	if(   dynamic_cast<const tree::Assign *>(&operation)
	   || dynamic_cast<const tree::Or *>(&operation)
	   || dynamic_cast<const tree::Xor *>(&operation)
	   || dynamic_cast<const tree::And *>(&operation)
	   || dynamic_cast<const tree::Add *>(&operation)
	   || dynamic_cast<const tree::Subtract *>(&operation)
	   || dynamic_cast<const tree::Multiply *>(&operation)
	   || dynamic_cast<const tree::Divide *>(&operation)
	   || dynamic_cast<const tree::Modulus *>(&operation)
	   || dynamic_cast<const tree::Not *>(&operation)
	   || dynamic_cast<const tree::Minus *>(&operation))
	{
		return true;
	}

	return false;
}

unsigned int tree::Int::getSizeInt() const
{
	ASSERT(isResolved());

	return mSize.intLiteral ? mSize.intLiteral->getValue() : tree::Int::DEFAULT_SIZE;
}

bool tree::Int::equals(const Type &type) const
{
	const Int *integer = dynamic_cast<const tree::Int *>(&type);

	if(integer)
	{
		return getSizeInt() == integer->getSizeInt();
	}
	else
	{
		return false;
	}
}

bool tree::Float::canCast(const tree::Type &from, bool autoCast) const
{
	const tree::Float *fromFloat;

	if(   dynamic_cast<const tree::Bool *>(&from)
	   || dynamic_cast<const tree::Int *>(&from)
	   || ((   fromFloat = dynamic_cast<const tree::Float *>(&from))
	        && fromFloat->getSize() <= getSize()))
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

bool tree::Float::canPerform(const Operation &operation) const
{
	if(   dynamic_cast<const tree::Assign *>(&operation)
	   || dynamic_cast<const tree::Add *>(&operation)
	   || dynamic_cast<const tree::Subtract *>(&operation)
	   || dynamic_cast<const tree::Multiply *>(&operation)
	   || dynamic_cast<const tree::Divide *>(&operation)
	   || dynamic_cast<const tree::Minus *>(&operation))
	{
		return true;
	}

	return false;
}

unsigned int tree::Float::getSizeInt() const
{
	ASSERT(isResolved());

	return mSize.intLiteral ? mSize.intLiteral->getValue() : tree::Float::DEFAULT_SIZE;
}

bool tree::Float::equals(const Type &type) const
{
	const Float *floatingPoint = dynamic_cast<const tree::Float *>(&type);

	if(floatingPoint)
	{
		return getSizeInt() == floatingPoint->getSizeInt();
	}
	else
	{
		return false;
	}
}

bool tree::String::canCast(const tree::Type &from, bool autoCast) const
{
	const tree::String *string;

	if((   string = dynamic_cast<const tree::String *>(&from))
	    && string->getSizeInt() <= getSizeInt())
	{
		return true;
	}

	return false;
}

unsigned int tree::String::getSizeInt() const
{
	ASSERT(isResolved());

	return mSize.intLiteral ? mSize.intLiteral->getValue() : tree::String::DEFAULT_SIZE;
}

bool tree::String::equals(const Type &type) const
{
	const String *string = dynamic_cast<const tree::String *>(&type);

	if(string)
	{
		return getSizeInt() == string->getSizeInt();
	}
	else
	{
		return false;
	}
}

tree::Array::Array(Type *type) : SizedType(new tree::None()), mType(type)
{
}

bool tree::Array::canCast(const tree::Type &from, bool autoCast) const
{
	const tree::Array *array;

	if((array = dynamic_cast<const tree::Array *>(&from)) && array->getSizeInt() <= getSizeInt())
	{
		return true;
	}

	return false;
}

unsigned int tree::Array::getSizeInt() const
{
	ASSERT(isResolved());

	return mSize.intLiteral->getValue();
}

bool tree::Array::equals(const Type &type) const
{
	const Array *array = dynamic_cast<const Array *>(&type);

	if(array)
	{
		return *getType() == *array->getType()  && getSizeInt() == array->getSizeInt();
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

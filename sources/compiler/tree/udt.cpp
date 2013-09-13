#include "common.h"

#include "compiler/tree/identifier.h"
#include "compiler/tree/udt.h"

void tree::UDT::checkNamedNode(const std::string &name, tree::Node *node)
{
	behaviour::NamedMap::NamedNodes::iterator previousValue = mNamedNodes.find(name);

	if(previousValue != mNamedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
}

tree::Node *tree::UDT::findNamedNode(tree::Identifier *identifier)
{
	const std::string &name = identifier->getName();
	std::map<std::string, Node *>::iterator identity = mNamedNodes.find(name);

	if(identity != mNamedNodes.end())
	{
		return identity->second;
	}
	else
	{
		throw behaviour::NamedMap::NotFoundException(identifier);
	}
}

void tree::UDT::mapNamedNode(const std::string &name, tree::Node *node)
{
	try
	{
		checkNamedNode(name, node);
		mNamedNodes[name] = node;
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



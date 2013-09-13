#include "common.h"

#include <string>
#include "compiler/tree/function.h"

void tree::Function::checkNamedNode(const std::string &name, tree::Node *node)
{
	behaviour::NamedMap::NamedNodes::iterator previousValue = mNamedNodes.find(name);

	if(previousValue != mNamedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
	else if(mOriginalScope)
	{
		mOriginalScope->checkNamedNode(name, node);
	}
	else if(mParent)
	{
		mParent->checkNamedNode(name, node);
	}
}

tree::Node *tree::Function::findNamedNode(tree::Identifier *identifier)
{
	const std::string &name = identifier->getName();
	std::map<std::string, Node *>::iterator identity = mNamedNodes.find(name);

	if(identity != mNamedNodes.end())
	{
		return identity->second;
	}
	else if(mOriginalScope)
	{
		return mOriginalScope->findNamedNode(identifier);
	}
	else if(mParent)
	{
		return mParent->findNamedNode(identifier);
	}
	else
	{
		throw behaviour::NamedMap::NotFoundException(identifier);
	}
}

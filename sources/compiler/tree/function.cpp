#include "common.h"

#include <string>
#include "compiler/tree/function.h"

void tree::Function::checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node)
{
	const behaviour::NamedMap::NamedNodes &namedNodes = mAssociatedNamedNodes[association];
	const behaviour::NamedMap::NamedNodes::const_iterator previousValue = namedNodes.find(name);

	if(previousValue != namedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
	else if(mOriginalScope)
	{
		mOriginalScope->checkAssociatedNamedNode(association, name, node);
	}
	else if(mParent)
	{
		mParent->checkAssociatedNamedNode(association, name, node);
	}
}

tree::Node *tree::Function::findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier)
{
	const behaviour::NamedMap::NamedNodes &namedNodes = mAssociatedNamedNodes[association];
	const std::string &name = identifier->getName();
	const behaviour::NamedMap::NamedNodes::const_iterator identity = namedNodes.find(name);

	if(identity != namedNodes.end())
	{
		return identity->second;
	}
	else if(mOriginalScope)
	{
		return mOriginalScope->findAssociatedNamedNode(association, identifier);
	}
	else if(mParent)
	{
		return mParent->findAssociatedNamedNode(association, identifier);
	}
	else
	{
		throw behaviour::NamedMap::NotFoundException(identifier);
	}
}

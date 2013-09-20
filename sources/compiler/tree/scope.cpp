#include "common.h"

#include <string>
#include "compiler/tree/scope.h"

void tree::Scope::checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node)
{
	const behaviour::NamedMap::NamedNodes &namedNodes = mAssociatedNamedNodes[association];
	const behaviour::NamedMap::NamedNodes::const_iterator previousValue = namedNodes.find(name);

	if(previousValue != namedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
	else if(mParent)
	{
		mParent->checkAssociatedNamedNode(association, name, node);
	}
}

tree::Node *tree::Scope::findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier)
{
	const behaviour::NamedMap::NamedNodes &namedNodes = mAssociatedNamedNodes[association];
	const std::string &name = identifier->getName();
	const behaviour::NamedMap::NamedNodes::const_iterator identity = namedNodes.find(name);

	if(identity != namedNodes.end())
	{
		return identity->second;
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

void tree::Scope::mapAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node)
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

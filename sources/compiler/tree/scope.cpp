#include <string>
#include "compiler/tree.h"


void tree::Scope::checkNamedNode(const std::string &name, tree::Node *node)
{
	behaviour::NamedMap::NamedNodes::iterator previousValue = mNamedNodes.find(name);

	if(previousValue != mNamedNodes.end())
	{
		throw behaviour::NamedMap::ExistsException(node, previousValue->second);
	}
	else if(mParent)
	{
		mParent->checkNamedNode(name, node);
	}
}

tree::Node *tree::Scope::findNamedNode(tree::Identifier *identifier)
{
	const std::string &name = identifier->getName();
	std::map<std::string, Node *>::iterator identity = mNamedNodes.find(name);

	if(identity != mNamedNodes.end())
	{
		return identity->second;
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

void tree::Scope::mapNamedNode(const std::string &name, tree::Node *node)
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

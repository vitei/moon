#include <string>
#include "compiler/tree.h"


void tree::Scope::checkNamedNode(const std::string &name, tree::Node *node)
{
	tree::Scope::NamedNodes::iterator previousValue = mNamedNodes.find(name);

	if(previousValue != mNamedNodes.end())
	{
		if(node != previousValue->second)
		{
			throw tree::Scope::ExistsException(node, previousValue->second);
		}
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
		throw tree::Scope::NotFoundException(identifier);
	}
}

void tree::Scope::mapNamedNode(const std::string &name, tree::Node *node)
{
	checkNamedNode(name, node);
	mNamedNodes[name] = node;
}

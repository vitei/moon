#include "common.h"

#include <string>
#include "compiler/tree/method.h"

void tree::Method::setType(tree::Type *type)
{
	tree::Type *t;

	if(type == NULL)
	{
		mType = NULL;
	}
	else if(tree::node_cast<tree::Identifier *>(type) ||
	        tree::node_cast<tree::Type *>(type))
	{
		mType = type;
	}
	else
	{
		throw tree::Method::InvalidTypeException(this);
	}
}

/*void tree::Function::checkNamedNode(const std::string &name, tree::Node *node)
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
}*/

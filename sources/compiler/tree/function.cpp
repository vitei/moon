#include <string>
#include "compiler/tree.h"


void tree::Function::checkIdentity(Identity *identity)
{
	const std::string &name = identity->getName();
	tree::Identities::iterator previousValue = mParameterIdentities.find(name);

	if(previousValue != mParameterIdentities.end())
	{
		throw tree::Scope::ExistsException(identity, previousValue->second);
	}
	else
	{
		tree::Scope::checkIdentity(identity);
	}
}

void tree::Function::mapParameterIdentity(tree::Identity *identity)
{
	checkIdentity(identity);
	mParameterIdentities[identity->getName()] = identity;
}

tree::Identity *tree::Function::findIdentity(tree::Identifier *identifier)
{
	const std::string &name = identifier->getName();
	tree::Identities::iterator identity = mParameterIdentities.find(name);

	if(identity != mParameterIdentities.end())
	{
		return identity->second;
	}
	else if(mParent)
	{
		return tree::Scope::findIdentity(identifier);
	}
	else
	{
		throw tree::Scope::NotFoundException(identifier);
	}
}

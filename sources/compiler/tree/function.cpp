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
		previousValue = mIdentities.find(name);

		if(previousValue != mIdentities.end())
		{
			throw tree::Scope::ExistsException(identity, previousValue->second);
		}
		else if(mOriginalScope)
		{
			mOriginalScope->checkIdentity(identity);
		}
		else if(mParent)
		{
			mParent->checkIdentity(identity);
		}
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
	else
	{
		identity = mIdentities.find(name);

		if(identity != mIdentities.end())
		{
			return identity->second;
		}
		else if(mOriginalScope)
		{
			return mOriginalScope->findIdentity(identifier);
		}
		else if(mParent)
		{
			return mParent->findIdentity(identifier);
		}
		else
		{
			throw tree::Scope::NotFoundException(identifier);
		}
	}
}

#include <string>
#include "compiler/tree.h"


void tree::Scope::mapIdentity(tree::Identity *identity)
{
	const std::string &name = identity->getName();

	for(Scope *s = this; s; s = s->mParent)
	{
		std::map<std::string, Identity *>::iterator previousValue = s->mIdentities.find(name);

		if(previousValue != s->mIdentities.end())
		{
			throw(tree::Scope::ExistsException(previousValue->second));
		}
	}

	mIdentities[name] = identity;
}

tree::Identity *tree::Scope::findIdentity(tree::Identifier *identifier)
{
	const std::string &name = identifier->getName();

	for(Scope *s = this; s; s = s->mParent)
	{
		std::map<std::string, Identity *>::iterator identity = s->mIdentities.find(name);

		if(identity != s->mIdentities.end())
		{
			return identity->second;
		}
	}

	throw(tree::Scope::NotFoundException());
}

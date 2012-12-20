#include <string>
#include "compiler/tree.h"


void tree::Scope::mapIdentity(tree::Identity *identity)
{
	const std::string &name = identity->getName()->getName();

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

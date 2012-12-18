#include <string>
#include "compiler/tree.h"


void tree::Scope::mapIdentity(tree::Identity *identity)
{
	std::string name = identity->getName()->getName();

	for(Scope *s = this; s; s = s->mParent)
	{
		if(s->mIdentities.find(name) != s->mIdentities.end())
		{
			 throw(new tree::Scope::ExistsException());
		}
	}

	mIdentities[name] = identity;
}
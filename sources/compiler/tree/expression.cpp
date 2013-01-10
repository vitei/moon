#include "compiler/tree.h"


void tree::FunctionCall::setFunctionPrototype(tree::Expression *functionPrototype)
{
	tree::FunctionPrototype *prototype;

	if(!functionPrototype)
	{
		mFunctionPrototype = NULL;
	}
	else if(dynamic_cast<tree::Identifier *>(functionPrototype))
	{
		mFunctionPrototype = functionPrototype;
	}
	else if((prototype = dynamic_cast<tree::FunctionPrototype *>(functionPrototype)))
	{
		if((!prototype->getArguments() && !mArguments) || prototype->getArguments()->size() == mArguments->size())
		{
			mFunctionPrototype = prototype;
		}
		else
		{
			mFunctionPrototype = NULL;
			throw(tree::FunctionCall::InvalidArgumentsException(this, prototype));
		}
	}
	else
	{
		mFunctionPrototype = NULL;
		throw(tree::FunctionCall::InvalidFunctionException(static_cast<tree::Identity *>(functionPrototype)));
	}
}

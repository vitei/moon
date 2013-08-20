#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::InferTypes::run(tree::Program *program)
{
	operation::InferTypes operation;
	program->accept(&operation);

	for(std::map<tree::TypedIdentity *, tree::Type *>::iterator i = operation.mTypeResolution.begin(), e = operation.mTypeResolution.end(); i != e; ++i)
	{
		if(i->second)
		{
			i->first->setType(i->second);
		}
	}
}

void operation::InferTypes::visit(tree::Access *access)
{
	LOG("InferTypes::visit::Access");

	if(!access->getType())
	{
		ASSERT(access->getTarget());

		tree::Type *type = access->getTarget()->getType();

		if(type)
		{
			access->setType(type);
		}
	}
}

void operation::InferTypes::visit(tree::Assign *assign)
{
	LOG("InferTypes::visit::Assign");

	tree::Expression *lhs = assign->getLHS();

	ASSERT(lhs);

	lhs->accept(this);

	tree::TypedIdentity *identity = dynamic_cast<tree::TypedIdentity *>(lhs);

	if(identity && !identity->getType())
	{
		ASSERT(assign->getRHS());

		mAssignIdentity = identity;
		mSelfReference = false;

		assign->getRHS()->accept(this);

		mAssignIdentity = NULL;

		if(!mSelfReference)
		{
			tree::Type *rhsType = assign->getRHS()->getType();

			if(   !rhsType                                                                     // If this type is not resolved then flag we can't process it
			   || mTypeResolution.find(identity) == mTypeResolution.end()                      // If this identity hasn't been added yet then add it
			   || (mTypeResolution[identity] && rhsType->canCast(*mTypeResolution[identity]))) // If there is a current type and the RHS type can handle it then use the RHS type
			{
				mTypeResolution[identity] = rhsType;
			}
		}
	}
	else
	{
		// FIXME, is this required???
		ASSERT(assign->getRHS());
		assign->getRHS()->accept(this);
	}
}

void operation::InferTypes::visit(tree::Identity *identity)
{
	LOG("InferTypes::visit::Identity");

	if(mAssignIdentity && mAssignIdentity == identity)
	{
		mSelfReference = true;
	}
}

void operation::InferTypes::visit(tree::Function *function)
{
	LOG("InferTypes::visit::Function");

	mPrototype = function->getPrototype();
	ASSERT(mPrototype);

	if(!mPrototype->getType())
	{
		mTypeResolution[mPrototype] = new tree::Void();
	}

	visit(static_cast<tree::Scope *>(function));
	mPrototype = NULL;
}

void operation::InferTypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

void operation::InferTypes::visit(tree::Return *returnStatement)
{
	LOG("InferTypes::visit::Return");

	// Check if we need to infer the return type
	if(!mPrototype->getType())
	{
		ASSERT(returnStatement->getReturn());
		tree::Type *returnType = returnStatement->getReturn()->getType();

		if(   !returnType                                                                         // If this type is not resolved then flag we can't process it
		   || (mTypeResolution[mPrototype] && returnType->canCast(*mTypeResolution[mPrototype]))) // If there is a current type and the return type can handle it then use the return type
		{
			mTypeResolution[mPrototype] = returnType;
		}
	}
}

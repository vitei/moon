#include "common.h"

#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/infer_types.h"

bool operation::InferTypes::run(tree::Program *program)
{
	operation::InferTypes operation;

	operation.mValidated = true;
	program->accept(&operation);

	for(std::map<tree::TypedIdentity *, tree::Type *>::iterator i = operation.mTypeResolution.begin(), e = operation.mTypeResolution.end(); i != e; ++i)
	{
		if(i->second)
		{
			i->first->setType(i->second);
		}
		else
		{
			operation.mValidated = false;
		}
	}

	if(operation.mValidated && !operation.mAccessedUnresolvedIdentities.empty())
	{
		for(std::set<tree::Identity *>::iterator i = operation.mAccessedUnresolvedIdentities.begin(), e = operation.mAccessedUnresolvedIdentities.end(); i != e; ++i)
		{
			// We need to check the type again because it might have been assigned one by now...
			if(!(*i)->getType())
			{
				std::string error = "The type of identifier \"" + (*i)->getName() + "\" could not be inferred";
				error::enqueue((*i)->getLocation(), error);
			}
		}
	}

	return operation.mValidated;
}

void operation::InferTypes::visit(tree::Assign *assign)
{
	LOG("InferTypes::visit::Assign");

	tree::Expression *lhs = assign->getLHS();

	ASSERT(lhs);

	lhs->accept(this);

	tree::TypedIdentity *identity = tree::node_cast<tree::TypedIdentity *>(lhs);

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

			// If the type is not resolved yet then we can't be sure about any type inference.
			if(rhsType && !rhsType->isResolved())
			{
				rhsType = NULL;
			}

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

void operation::InferTypes::visit(tree::For *forStatement)
{
	LOG("InferTypes::visit::For");

	tree::Variable *variable = forStatement->getVariable();

	ASSERT(variable);

	if(!variable->getType())
	{
		tree::Expression *iterable = forStatement->getIterable();
		ASSERT(iterable);
		tree::Type *type = iterable->getType();

		if(type)
		{
#ifndef DEBUG
			tree::Array *array = static_cast<tree::Array *>(type);
#else
			tree::Array *array = tree::node_cast<tree::Array *>(type);
			ASSERT(array);
#endif

			variable->setType(array->getType());
		}

		if(!variable->getType())
		{
			mValidated = false;
		}
	}

	visit(static_cast<tree::Scope *>(forStatement));
}

void operation::InferTypes::visit(tree::Identity *identity)
{
	LOG("InferTypes::visit::Identity");

	if(mAssignIdentity && mAssignIdentity == identity)
	{
		mSelfReference = true;
	}

	if(!identity->getType())
	{
		mAccessedUnresolvedIdentities.insert(identity);
	}
}

void operation::InferTypes::visit(tree::Identifier *identifier)
{
	LOG("InferTypes::visit::Identifier");

	// Can't be sure of types when there are still identifiers...
	mValidated = false;
}

void operation::InferTypes::visit(tree::Function *function)
{
	LOG("InferTypes::visit::Function");

	mPrototype = function->getPrototype();
	ASSERT(mPrototype);

	visit(static_cast<tree::Scope *>(function));

	if(!mPrototype->getType() && mTypeResolution.find(mPrototype) == mTypeResolution.end())
	{
		mTypeResolution[mPrototype] = new tree::Void();
	}

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
		   || mTypeResolution.find(mPrototype) == mTypeResolution.end()                           // If the return type hasn't been added yet then add it
		   || (mTypeResolution[mPrototype] && returnType->canCast(*mTypeResolution[mPrototype]))) // If there is a current type and the return type can handle it then use the return type
		{
			mTypeResolution[mPrototype] = returnType;
		}
	}
}

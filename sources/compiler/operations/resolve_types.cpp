#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ResolveTypes::run(tree::Program *program)
{
	for(bool resolved = false; !resolved;)
	{
		operation::ResolveTypes operation;
		program->accept(&operation);
		resolved = operation.resolve();
	}
}

void operation::ResolveTypes::visit(tree::Access *access)
{
	LOG("ResolveTypes::visit::Access");

	if(!access->getType())
	{
		ASSERT(access->getTarget());
		access->setType(access->getTarget()->getType());
	}
}

void operation::ResolveTypes::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("ResolveTypes::visit::ArrayAccess");

	if(!arrayAccess->getType())
	{
		ASSERT(arrayAccess->getContainer());
		arrayAccess->setType(arrayAccess->getContainer()->getType());
	}
}

void operation::ResolveTypes::visit(tree::BinaryExpression *binaryExpression)
{
	LOG("ResolveTypes::visit::BinaryExpression");

	if(!binaryExpression->getType())
	{
		ASSERT(binaryExpression->getLHS());
		ASSERT(binaryExpression->getRHS());

		tree::Type *lhsType = binaryExpression->getLHS()->getType();
		tree::Type *rhsType = binaryExpression->getRHS()->getType();

		if(!lhsType || !rhsType)
		{
			binaryExpression->setType(NULL);
		}
		else
		{
			ASSERT(lhsType);
			ASSERT(rhsType);

			binaryExpression->setType(*lhsType > *rhsType ? lhsType : rhsType);
		}
	}
}

void operation::ResolveTypes::visit(tree::Assign *assign)
{
	LOG("ResolveTypes::visit::Assign");

	tree::Expression *lhs = assign->getLHS();

	ASSERT(lhs);
	ASSERT(assign->getRHS());

	tree::Type *lhsType = lhs->getType();
	tree::Type *rhsType = assign->getRHS()->getType();

	if(!lhsType)
	{
		if(mTypeResolution.find(lhs) == mTypeResolution.end())
		{
			mTypeResolution[lhs] = assign->getRHS()->getType();
		}
		else if(mTypeResolution[lhs])
		{
			if(!rhsType)
			{
				mTypeResolution[lhs] = NULL;
			}
			else if(*rhsType > *mTypeResolution[lhs])
			{
				mTypeResolution[lhs] = rhsType;
			}
		}
	}
	else
	{
		assign->setType(lhsType);
	}
}

void operation::ResolveTypes::visit(tree::BooleanBinaryExpression *booleanBinaryExpression)
{
	LOG("ResolveTypes::visit::BooleanBinaryExpression");

	ASSERT(booleanBinaryExpression->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanBinaryExpression->getType()));
}

void operation::ResolveTypes::visit(tree::UnaryExpression *unaryExpression)
{
	LOG("ResolveTypes::visit::UnaryExpression");

	if(!unaryExpression->getType())
	{
		ASSERT(unaryExpression->getExpression());
		unaryExpression->setType(unaryExpression->getExpression()->getType());
	}
}

void operation::ResolveTypes::visit(tree::BooleanUnaryExpression *booleanUnaryExpression)
{
	LOG("ResolveTypes::visit::BooleanUnaryExpression");

	ASSERT(booleanUnaryExpression->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanUnaryExpression->getType()));
}

void operation::ResolveTypes::visit(tree::FunctionCall *functionCall)
{
	LOG("ResolveTypes::visit::FunctionCall");

	if(!functionCall->getType())
	{
		ASSERT(functionCall->getPrototype());

		tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());
		functionCall->setType(functionPrototype->getType());
	}
}

void operation::ResolveTypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

void operation::ResolveTypes::visit(tree::Function *function)
{
	LOG("ResolveTypes::visit::Function");

	if(function->getPrototype() && function->getPrototype()->getType() == NULL)
	{
		mPrototype = function->getPrototype();
		mTypeResolution[mPrototype] = new tree::Void();
	}

	visit(static_cast<tree::Scope *>(function));
	mPrototype = NULL;
}

void operation::ResolveTypes::visit(tree::Return *returnStatement)
{
	LOG("ResolveTypes::visit::Return");

	if(mPrototype)
	{
		// Ensure there was no ambiguity
		if(mTypeResolution[mPrototype])
		{
			// Check there is a return
			if(returnStatement->getReturn())
			{
				// Ensure this return will not generate ambiguity
				if(returnStatement->getReturn()->getType())
				{
					if(*returnStatement->getReturn()->getType() > *mTypeResolution[mPrototype])
					{
						mTypeResolution[mPrototype] = returnStatement->getReturn()->getType();
					}
				}
				else
				{
					mTypeResolution[mPrototype] = NULL;
				}
			}
		}
	}
}

bool operation::ResolveTypes::resolve()
{
	if(mTypeResolution.begin() != mTypeResolution.end())
	{
		for(std::map<tree::Expression *, tree::Type *>::iterator i = mTypeResolution.begin(), e = mTypeResolution.end(); i != e; ++i)
		{
			i->first->setType(i->second);
		}

		LOG("ResolveTypes new pass required...");

		return false;
	}
	else
	{
		return true;
	}
}

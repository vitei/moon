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

	if(!arrayAccess->getType() && arrayAccess->getContainer())
	{
		tree::Type *type = arrayAccess->getContainer()->getType();

		if(type)
		{
			tree::Array *arrayType = dynamic_cast<tree::Array *>(type);

			if(arrayType)
			{
				arrayAccess->setType(arrayType->getType());
			}
			else
			{
				error::enqueue(arrayAccess->getLocation(), "Expression result is not an array");
				arrayAccess->setContainer(NULL);
			}
		}
	}
}

void operation::ResolveTypes::visit(tree::BinaryOperation *binaryOperation)
{
	LOG("ResolveTypes::visit::BinaryOperation");

	if(!binaryOperation->getType())
	{
		ASSERT(binaryOperation->getLHS());
		ASSERT(binaryOperation->getRHS());

		tree::Type *lhsType = binaryOperation->getLHS()->getType();
		tree::Type *rhsType = binaryOperation->getRHS()->getType();

		if(!lhsType || !rhsType)
		{
			binaryOperation->setType(NULL);
		}
		else
		{
			ASSERT(lhsType);
			ASSERT(rhsType);

			binaryOperation->setType(lhsType->canCast(*rhsType) ? lhsType : rhsType);
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
		if(dynamic_cast<tree::TypedIdentity *>(lhs))
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
				else if(rhsType->canCast(*mTypeResolution[lhs]))
				{
					mTypeResolution[lhs] = rhsType;
				}
			}
		}
	}
	else
	{
		assign->setType(lhsType);
	}
}

void operation::ResolveTypes::visit(tree::BooleanBinaryOperation *booleanBinaryOperation)
{
	LOG("ResolveTypes::visit::BooleanBinaryOperation");

	ASSERT(booleanBinaryOperation->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanBinaryOperation->getType()));
}

void operation::ResolveTypes::visit(tree::UnaryOperation *unaryOperation)
{
	LOG("ResolveTypes::visit::UnaryOperation");

	if(!unaryOperation->getType())
	{
		ASSERT(unaryOperation->getExpression());
		unaryOperation->setType(unaryOperation->getExpression()->getType());
	}
}

void operation::ResolveTypes::visit(tree::BooleanUnaryOperation *booleanUnaryOperation)
{
	LOG("ResolveTypes::visit::BooleanUnaryOperation");

	ASSERT(booleanUnaryOperation->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanUnaryOperation->getType()));
}

void operation::ResolveTypes::visit(tree::FunctionCall *functionCall)
{
	LOG("ResolveTypes::visit::FunctionCall");

	tree::Expressions *arguments = functionCall->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; (*i++)->accept(this));
	}

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
					tree::Type *returnType = returnStatement->getReturn()->getType();
					if(returnType->canCast(*mTypeResolution[mPrototype]))
					{
						mTypeResolution[mPrototype] = returnType;
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

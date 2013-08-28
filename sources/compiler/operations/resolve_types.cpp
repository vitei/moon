#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ResolveTypes::run(tree::Program *program)
{
	operation::ResolveTypes operation;
	program->accept(&operation);
}

void operation::ResolveTypes::visit(tree::Access *access)
{
	LOG("ResolveTypes::visit::Access");

	if(!access->getType())
	{
		ASSERT(access->getTarget());

		tree::Type *type = access->getTarget()->getType();

		if(type && type->isResolved())
		{
			access->setType(type);
		}
	}
}

void operation::ResolveTypes::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("ResolveTypes::visit::ArrayAccess");

	if(!arrayAccess->getType())
	{
		ASSERT(arrayAccess->getContainer());

		tree::Type *type = arrayAccess->getContainer()->getType();

		if(type && type->isResolved())
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

		if(lhsType && lhsType->isResolved() && rhsType && rhsType->isResolved())
		{
			setOperationType(binaryOperation, lhsType->canCast(*rhsType) ? lhsType : rhsType);
		}
	}
}

void operation::ResolveTypes::visit(tree::Assign *assign)
{
	LOG("ResolveTypes::visit::Assign");

	if(!assign->getType())
	{
		ASSERT(assign->getLHS());

		tree::Type *lhsType = assign->getLHS()->getType();

		if(lhsType && lhsType->isResolved())
		{
			setOperationType(assign, lhsType);
		}
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

		tree::Type *type = unaryOperation->getExpression()->getType();

		if(type && type->isResolved())
		{
			setOperationType(unaryOperation, type);
		}
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
		tree::Type *type = functionPrototype->getType();

		if(type && type->isResolved())
		{
			functionCall->setType(type);
		}
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

void operation::ResolveTypes::setOperationType(tree::Operation *operation, tree::Type *type)
{
	try
	{
		operation->setType(type);
	}
	catch(tree::Operation::NotAllowedException &e)
	{
		std::string error = std::string("Operation is not valid for ") + type->getTypeName() + " type";
		error::enqueue(e.expression->getLocation(), error);

		e.reset();
	}
}

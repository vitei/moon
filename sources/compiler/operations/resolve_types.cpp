#include "common.h"

#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/resolve_types.h"

bool operation::ResolveTypes::run(tree::Program *program)
{
	operation::ResolveTypes operation;

	operation.mValidated = true;
	program->accept(&operation);

	return operation.mValidated;
}

void operation::ResolveTypes::visit(tree::Access *access)
{
	LOG("ResolveTypes::visit::Access");

	if(!access->getType())
	{
		ASSERT(access->getTarget());

		tree::Expression *target = tree::node_cast<tree::Expression *>(access->getTarget());

		if(target)
		{
			tree::Type *type = target->getType();

			if(type && type->isResolved())
			{
				access->setType(type);
			}

			if(!access->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
}

void operation::ResolveTypes::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("ResolveTypes::visit::ArrayAccess");

	if(!arrayAccess->getType())
	{
		ASSERT(arrayAccess->getContainer());

		tree::Expression *container = tree::node_cast<tree::Expression *>(arrayAccess->getContainer());

		if(container)
		{
			tree::Type *type = container->getType();

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

			if(!arrayAccess->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
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

		tree::Expression *lhs = tree::node_cast<tree::Expression *>(binaryOperation->getLHS());
		tree::Expression *rhs = tree::node_cast<tree::Expression *>(binaryOperation->getRHS());

		if(lhs && rhs)
		{
			tree::Type *lhsType = lhs->getType();
			tree::Type *rhsType = rhs->getType();

			if(lhsType && lhsType->isResolved() && rhsType && rhsType->isResolved())
			{
				setOperationType(binaryOperation, lhsType->canCast(*rhsType) ? lhsType : rhsType);
			}

			if(!binaryOperation->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
}

void operation::ResolveTypes::visit(tree::Assign *assign)
{
	LOG("ResolveTypes::visit::Assign");

	if(!assign->getType())
	{
		ASSERT(assign->getLHS());

		tree::Expression *lhs = tree::node_cast<tree::Expression *>(assign->getLHS());

		if(lhs)
		{
			tree::Type *lhsType = lhs->getType();

			if(lhsType && lhsType->isResolved())
			{
				setOperationType(assign, lhsType);
			}

			if(!assign->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
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

		tree::Expression *expression = tree::node_cast<tree::Expression *>(unaryOperation->getExpression());

		if(expression)
		{
			tree::Type *type = expression->getType();

			if(type && type->isResolved())
			{
				setOperationType(unaryOperation, type);
			}

			if(!unaryOperation->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
}

void operation::ResolveTypes::visit(tree::BooleanUnaryOperation *booleanUnaryOperation)
{
	LOG("ResolveTypes::visit::BooleanUnaryOperation");

	ASSERT(booleanUnaryOperation->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanUnaryOperation->getType()));
}

void operation::ResolveTypes::visit(tree::ComputedArray *computedArray)
{
	LOG("ResolveTypes::visit::ComputedArray");

	if(!computedArray->getType())
	{
		tree::Literal *fromLiteral = tree::node_cast<tree::Literal *>(computedArray->getFrom());
		tree::Literal *toLiteral = tree::node_cast<tree::Literal *>(computedArray->getTo());

		if(fromLiteral && toLiteral)
		{
			if(   !computedArray->getStep()
			   || tree::node_cast<tree::Literal *>(computedArray->getTo()))

			{
				// FIXME, this is rubbish ATM...
				// // It needs to take types into account properly...
				tree::IntLiteral *from = dynamic_cast<tree::IntLiteral *>(fromLiteral);
				tree::IntLiteral *to = dynamic_cast<tree::IntLiteral *>(toLiteral);

				ASSERT(from);
				ASSERT(to);

				int numValues = to->getValue() - from->getValue();

				if(computedArray->getStep())
				{
					tree::IntLiteral *step = dynamic_cast<tree::IntLiteral *>(computedArray->getStep());

					ASSERT(step);

					numValues /= step->getValue();
				}

				computedArray->setType(new tree::Array(new tree::Int(), new tree::IntLiteral(numValues)));
			}
			else
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
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

		tree::FunctionPrototype *functionPrototype = tree::node_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

		if(functionPrototype)
		{
			tree::Type *type = functionPrototype->getType();

			if(type && type->isResolved())
			{
				functionCall->setType(type);
			}

			if(!functionCall->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
}

void operation::ResolveTypes::visit(tree::IfExpression *ifExpression)
{
	LOG("ResolveTypes::visit::IfExpression");

	if(!ifExpression->getType())
	{
		ASSERT(ifExpression->getTrueResult());
		ASSERT(ifExpression->getFalseResult());

		tree::Expression *trueResult = tree::node_cast<tree::Expression *>(ifExpression->getTrueResult());
		tree::Expression *falseResult = tree::node_cast<tree::Expression *>(ifExpression->getFalseResult());

		if(trueResult && falseResult)
		{
			tree::Type *trueResultType = trueResult->getType();
			tree::Type *falseResultType = falseResult->getType();

			if(trueResultType && trueResultType->isResolved() && falseResultType && falseResultType->isResolved())
			{
				ifExpression->setType(trueResultType->canCast(*falseResultType) ? trueResultType : falseResultType);
			}

			if(!ifExpression->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			mValidated = false;
		}
	}
}

void operation::ResolveTypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		tree::Scope *oldScope = mCurrentScope;

		mCurrentScope = scope;
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this))
			;
		mCurrentScope = oldScope;
	}
}

void operation::ResolveTypes::visit(tree::This *th1s)
{
	LOG("ResolveTypes::visit::This");

	if(!th1s->getType())
	{
		tree::Method *method = tree::node_cast<tree::Method *>(mCurrentScope);

		if(method)
		{
			tree::Type *type = method->getType();

			if(type && type->isResolved())
			{
				th1s->setType(type);
			}

			if(!th1s->getType())
			{
				mValidated = false;
			}
		}
		else
		{
			error::enqueue(th1s->getLocation(), "Scope is not a method");
		}
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

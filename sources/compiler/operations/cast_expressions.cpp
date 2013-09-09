#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


bool operation::CastExpressions::run(tree::Program *program)
{
	operation::CastExpressions operation;

	operation.mValidated = true;
	program->accept(&operation);

	return operation.mValidated;
}

void operation::CastExpressions::visit(tree::BinaryOperation *binaryOperation)
{
	LOG("CastExpressions::visit::BinaryOperation");

	ASSERT(binaryOperation->getLHS());
	ASSERT(binaryOperation->getRHS());

	tree::Type *operationType = binaryOperation->getType();

	if(operationType && operationType->isResolved())
	{
		tree::Type *lhsType = binaryOperation->getLHS()->getType();
		tree::Type *rhsType = binaryOperation->getRHS()->getType();

		if(lhsType && lhsType->isResolved())
		{
			if(*lhsType != *operationType)
			{
				tree::Cast *cast = new tree::Cast(operationType, binaryOperation->getLHS(), true);

				cast->setLocation(binaryOperation->getLocation());
				binaryOperation->setLHS(cast);
			}
		}
		else
		{
			mValidated = false;
		}

		if(rhsType && rhsType->isResolved())
		{
			if(*rhsType != *operationType)
			{
				tree::Cast *cast = new tree::Cast(operationType, binaryOperation->getRHS(), true);

				cast->setLocation(binaryOperation->getLocation());
				binaryOperation->setRHS(cast);
			}
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

void operation::CastExpressions::visit(tree::BooleanBinaryOperation *booleanBinaryOperation)
{
	LOG("CastExpressions::visit::BooleanBinaryOperation");

	ASSERT(booleanBinaryOperation->getLHS());
	ASSERT(booleanBinaryOperation->getRHS());

	tree::Type *typeA = booleanBinaryOperation->getLHS()->getType();
	tree::Type *typeB = booleanBinaryOperation->getRHS()->getType();

	if(typeA && typeA->isResolved() && typeB && typeB->isResolved())
	{
		if(*typeA != *typeB)
		{
			if(typeA->canCast(*typeB, true))
			{
				tree::Cast *cast = new tree::Cast(typeA, booleanBinaryOperation->getRHS(), true);

				cast->setLocation(booleanBinaryOperation->getLocation());
				booleanBinaryOperation->setRHS(cast);
			}
			else
			{
				tree::Cast *cast = new tree::Cast(typeB, booleanBinaryOperation->getLHS(), true);

				cast->setLocation(booleanBinaryOperation->getLocation());
				booleanBinaryOperation->setLHS(cast);
			}
		}
	}
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("CastExpressions::visit::FunctionCall");

	ASSERT(functionCall->getPrototype());

	tree::FunctionPrototype *functionPrototype = dynamic_cast<tree::FunctionPrototype *>(static_cast<tree::Node *>(functionCall->getPrototype()));

	if(functionPrototype)
	{
		tree::Expressions *arguments = functionCall->getArguments();
		tree::Expressions *parameters = functionPrototype->getArguments();

		if(arguments && parameters)
		{
			for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(), j = parameters->begin(); i != end; ++i, ++j)
			{
				// Ensure the parameter gets typed...
				(*i)->accept(this);

				tree::Type *expectedType = (*j)->getType();
				tree::Type *actualType = (*i)->getType();

				if(expectedType && expectedType->isResolved() && actualType && actualType->isResolved())
				{
					// Check the types in-case unresolved
					if(*expectedType != *actualType)
					{
						tree::Cast *cast = new tree::Cast(expectedType, *i, true);

						cast->setLocation((*i)->getLocation());
						*i = cast;
					}
				}
				else
				{
					mValidated = false;
				}
			}
		}
	}
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

void operation::CastExpressions::visit(tree::Function *function)
{
	mPrototype = function->getPrototype();
	visit(static_cast<tree::Scope *>(function));
	mPrototype = NULL;
}

void operation::CastExpressions::visit(tree::If *ifStatement)
{
	LOG("CastExpressions::setup::If");

	tree::Expression *test = ifStatement->getTest();

	ASSERT(test);

	tree::Type *type = test->getType();

	if(type && type->isResolved())
	{
		if(!dynamic_cast<tree::Bool *>(type))
		{
#ifdef DEBUG
			type->printType();
#endif

			tree::Cast *cast = new tree::Cast(new tree::Bool(), test, true);

			cast->setLocation(test->getLocation());
			ifStatement->setTest(cast);
		}
	}
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::While *whileStatement)
{
	LOG("CastExpressions::visit::While");

	tree::Expression *test = whileStatement->getTest();

	ASSERT(test);

	tree::Type *type = test->getType();

	if(type && type->isResolved())
	{
		if(!dynamic_cast<tree::Bool *>(type))
		{
#ifdef DEBUG
			type->printType();
#endif

			tree::Cast *cast = new tree::Cast(new tree::Bool(), test, true);

			cast->setLocation(test->getLocation());
			whileStatement->setTest(cast);
		}
	}
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::Return *returnStatement)
{
	LOG("CastExpressions::visit::Return");

	tree::Type *type = mPrototype->getType();
	tree::Expression *returnExpression = returnStatement->getReturn();

	if(type && type->isResolved())
	{
		if(!returnExpression)
		{
			if(*type != tree::Void())
			{
				std::string error = "All return statements for function \"" + mPrototype->getName() + "\" must return a value";
				error::enqueue(returnStatement->getLocation(), error);
			}
		}
		else
		{
			tree::Type *returnType = returnExpression->getType();

			if(returnType && returnType->isResolved())
			{
				if(*returnType != *type)
				{
					tree::Cast *cast = new tree::Cast(type, returnExpression, true);

					cast->setLocation(returnStatement->getLocation());
					returnStatement->setReturn(cast);
				}
			}
			else
			{
				mValidated = false;
			}
		}
	}
	else
	{
		mValidated = false;
	}
}

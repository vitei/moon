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

	tree::Type *operationType = binaryOperation->getType();

	if(operationType && operationType->isResolved())
	{
		ASSERT(binaryOperation->getLHS());
		ASSERT(binaryOperation->getRHS());

		tree::Expression *lhs = tree::node_cast<tree::Expression *>(binaryOperation->getLHS());
		tree::Expression *rhs = tree::node_cast<tree::Expression *>(binaryOperation->getRHS());

		if(lhs && rhs)
		{
			tree::Type *lhsType = lhs->getType();
			tree::Type *rhsType = rhs->getType();

			if(lhsType && lhsType->isResolved())
			{
				if(*lhsType != *operationType)
				{
					tree::Cast *cast = new tree::Cast(operationType, lhs, true);

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
					tree::Cast *cast = new tree::Cast(operationType, rhs, true);

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

	tree::Expression *lhs = tree::node_cast<tree::Expression *>(booleanBinaryOperation->getLHS());
	tree::Expression *rhs = tree::node_cast<tree::Expression *>(booleanBinaryOperation->getRHS());

	if(lhs && rhs)
	{
		tree::Type *typeA = lhs->getType();
		tree::Type *typeB = rhs->getType();

		if(typeA && typeA->isResolved() && typeB && typeB->isResolved())
		{
			if(*typeA != *typeB)
			{
				if(typeA->canCast(*typeB, true))
				{
					tree::Cast *cast = new tree::Cast(typeA, rhs, true);

					cast->setLocation(booleanBinaryOperation->getLocation());
					booleanBinaryOperation->setRHS(cast);
				}
				else
				{
					tree::Cast *cast = new tree::Cast(typeB, lhs, true);

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
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("CastExpressions::visit::FunctionCall");

	ASSERT(functionCall->getPrototype());

	tree::FunctionPrototype *functionPrototype = tree::node_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

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

	ASSERT(ifStatement->getTest());

	tree::Expression *test = tree::node_cast<tree::Expression *>(ifStatement->getTest());

	if(test)
	{
		tree::Type *type = test->getType();

		if(type && type->isResolved())
		{
			if(!dynamic_cast<tree::Bool *>(type))
			{
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
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::While *whileStatement)
{
	LOG("CastExpressions::visit::While");

	ASSERT(whileStatement->getTest());

	tree::Expression *test = tree::node_cast<tree::Expression *>(whileStatement->getTest());

	if(test)
	{
		tree::Type *type = test->getType();

		if(type && type->isResolved())
		{
			if(!dynamic_cast<tree::Bool *>(type))
			{
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
	else
	{
		mValidated = false;
	}
}

void operation::CastExpressions::visit(tree::Return *returnStatement)
{
	LOG("CastExpressions::visit::Return");

	tree::Type *type = mPrototype->getType();

	if(type && type->isResolved())
	{
		if(!returnStatement->getReturn())
		{
			if(*type != tree::Void())
			{
				std::string error = "All return statements for function \"" + mPrototype->getName() + "\" must return a value";
				error::enqueue(returnStatement->getLocation(), error);
			}
		}
		else
		{
			tree::Expression *returnExpression = tree::node_cast<tree::Expression *>(returnStatement->getReturn());

			if(returnExpression)
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

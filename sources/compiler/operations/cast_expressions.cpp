#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::CastExpressions::run(tree::Program *program)
{
	operation::CastExpressions operation;
	program->accept(&operation);
}

void operation::CastExpressions::visit(tree::BinaryOperation *binaryOperation)
{
	LOG("CastExpressions::visit::BinaryOperation");

	ASSERT(binaryOperation->getType());

	if(binaryOperation->getLHS() && *binaryOperation->getLHS()->getType() != *binaryOperation->getType())
	{
		tree::Cast *cast = new tree::Cast(binaryOperation->getType(), binaryOperation->getLHS(), true);

		cast->setLocation(binaryOperation->getLocation());
		binaryOperation->setLHS(cast);
	}

	if(binaryOperation->getRHS() && *binaryOperation->getRHS()->getType() != *binaryOperation->getType())
	{
		tree::Cast *cast = new tree::Cast(binaryOperation->getType(), binaryOperation->getRHS(), true);

		cast->setLocation(binaryOperation->getLocation());
		binaryOperation->setRHS(cast);
	}
}

void operation::CastExpressions::visit(tree::BooleanBinaryOperation *booleanBinaryOperation)
{
	LOG("CastExpressions::visit::BooleanBinaryOperation");

	// Check the left and right types are the same
	if(booleanBinaryOperation->getLHS() && booleanBinaryOperation->getRHS())
	{
		tree::Type *typeA = booleanBinaryOperation->getLHS()->getType();
		tree::Type *typeB = booleanBinaryOperation->getRHS()->getType();

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
}

void operation::CastExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("CastExpressions::visit::FunctionCall");

	ASSERT(functionCall->getType());
	ASSERT(functionCall->getPrototype());

	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

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

			ASSERT(expectedType);
			ASSERT(actualType);

			// Check the types in-case unresolved
			if(*expectedType != *actualType)
			{
				tree::Cast *cast = new tree::Cast(expectedType, *i, true);

				cast->setLocation((*i)->getLocation());
				*i = cast;
			}
		}
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
	ASSERT(test->getType());

	if(!dynamic_cast<tree::Bool *>(test->getType()))
	{
#ifdef DEBUG
		test->getType()->printType();
#endif

		tree::Cast *cast = new tree::Cast(new tree::Bool(), test, true);

		cast->setLocation(test->getLocation());
		ifStatement->setTest(cast);
	}
}

void operation::CastExpressions::visit(tree::While *whileStatement)
{
	LOG("CastExpressions::visit::While");

	tree::Expression *test = whileStatement->getTest();

	ASSERT(test);
	ASSERT(test->getType());

	if(!dynamic_cast<tree::Bool *>(test->getType()))
	{
#ifdef DEBUG
		test->getType()->printType();
#endif

		tree::Cast *cast = new tree::Cast(new tree::Bool(), test, true);

		cast->setLocation(test->getLocation());
		whileStatement->setTest(cast);
	}
}

void operation::CastExpressions::visit(tree::Return *returnStatement)
{
	LOG("CastExpressions::visit::Return");

	ASSERT(mPrototype->getType());

	if(!returnStatement->getReturn())
	{
		if(*mPrototype->getType() != tree::Void())
		{
			std::string error = "All return statements for function \"" + mPrototype->getName() + "\" must return a value";
			error::enqueue(returnStatement->getLocation(), error);
		}
	}
	else if(*returnStatement->getReturn()->getType() != *mPrototype->getType())
	{
		tree::Cast *cast = new tree::Cast(mPrototype->getType(), returnStatement->getReturn(), true);

		cast->setLocation(returnStatement->getLocation());
		returnStatement->setReturn(cast);
	} 
}


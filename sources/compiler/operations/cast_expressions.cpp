#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


bool operation::CastExpressions::run(tree::Program *program)
{
	operation::CastExpressions operation;
	program->accept(&operation);

	return true; // The other operations should catch the unresolved types...
}

void operation::CastExpressions::visit(tree::BinaryOperation *binaryOperation)
{
	LOG("CastExpressions::visit::BinaryOperation");

	ASSERT(binaryOperation->getLHS());
	ASSERT(binaryOperation->getRHS());

	if(binaryOperation->getType())
	{
		if(binaryOperation->getLHS()->getType())
		{
			if(*binaryOperation->getLHS()->getType() != *binaryOperation->getType())
			{
				tree::Cast *cast = new tree::Cast(binaryOperation->getType(), binaryOperation->getLHS(), true);

				cast->setLocation(binaryOperation->getLocation());
				binaryOperation->setLHS(cast);
			}
		}

		if(binaryOperation->getRHS()->getType())
		{
			if(*binaryOperation->getRHS()->getType() != *binaryOperation->getType())
			{
				tree::Cast *cast = new tree::Cast(binaryOperation->getType(), binaryOperation->getRHS(), true);

				cast->setLocation(binaryOperation->getLocation());
				binaryOperation->setRHS(cast);
			}
		}
	}
}

void operation::CastExpressions::visit(tree::BooleanBinaryOperation *booleanBinaryOperation)
{
	LOG("CastExpressions::visit::BooleanBinaryOperation");

	ASSERT(booleanBinaryOperation->getLHS());
	ASSERT(booleanBinaryOperation->getRHS());

	tree::Type *typeA = booleanBinaryOperation->getLHS()->getType();
	tree::Type *typeB = booleanBinaryOperation->getRHS()->getType();

	if(typeA && typeB)
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
}

void operation::CastExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("CastExpressions::visit::FunctionCall");

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

			if(expectedType && actualType)
			{
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

	if(type)
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
}

void operation::CastExpressions::visit(tree::While *whileStatement)
{
	LOG("CastExpressions::visit::While");

	tree::Expression *test = whileStatement->getTest();

	ASSERT(test);

	tree::Type *type = test->getType();

	if(type)
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
}

void operation::CastExpressions::visit(tree::Return *returnStatement)
{
	LOG("CastExpressions::visit::Return");

	tree::Type *type = mPrototype->getType();
	tree::Expression *returnExpression = returnStatement->getReturn();

	if(type)
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

			if(returnType)
			{
				if(*returnType != *type)
				{
					tree::Cast *cast = new tree::Cast(type, returnExpression, true);

					cast->setLocation(returnStatement->getLocation());
					returnStatement->setReturn(cast);
				}
			}
		}
	}
}

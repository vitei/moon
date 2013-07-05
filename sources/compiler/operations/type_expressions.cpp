#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::TypeExpressions::run(tree::Program *program)
{
	operation::TypeExpressions operation;
	program->accept(&operation);
}

void operation::TypeExpressions::visit(tree::BinaryExpression *binaryExpression)
{
	LOG("TypeExpressions::visit::BinaryExpression");

	ASSERT(binaryExpression->getType());

	if(binaryExpression->getLHS() && *binaryExpression->getLHS()->getType() != *binaryExpression->getType())
	{
		tree::Cast *cast = new tree::Cast(binaryExpression->getType(), binaryExpression->getLHS(), true);

		cast->setLocation(binaryExpression->getLocation());
		binaryExpression->setLHS(cast);
	}

	if(binaryExpression->getRHS() && *binaryExpression->getRHS()->getType() != *binaryExpression->getType())
	{
		tree::Cast *cast = new tree::Cast(binaryExpression->getType(), binaryExpression->getRHS(), true);

		cast->setLocation(binaryExpression->getLocation());
		binaryExpression->setRHS(cast);
	}
}

void operation::TypeExpressions::visit(tree::BooleanBinaryExpression *booleanBinaryExpression)
{
	LOG("TypeExpressions::visit::BooleanBinaryExpression");

	// Check the left and right types are the same
	if(booleanBinaryExpression->getLHS() && booleanBinaryExpression->getRHS())
	{
		tree::Type *typeA = booleanBinaryExpression->getLHS()->getType();
		tree::Type *typeB = booleanBinaryExpression->getRHS()->getType();

		if(*typeA != *typeB)
		{
			if(typeA->canCast(*typeB, true))
			{
				tree::Cast *cast = new tree::Cast(typeA, booleanBinaryExpression->getRHS(), true);

				cast->setLocation(booleanBinaryExpression->getLocation());
				booleanBinaryExpression->setRHS(cast);
			}
			else
			{
				tree::Cast *cast = new tree::Cast(typeB, booleanBinaryExpression->getLHS(), true);

				cast->setLocation(booleanBinaryExpression->getLocation());
				booleanBinaryExpression->setLHS(cast);
			}
		}
	}
}

void operation::TypeExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("TypeExpressions::visit::FunctionCall");

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

void operation::TypeExpressions::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; (*i++)->accept(this));
	}
}

void operation::TypeExpressions::visit(tree::Function *function)
{
	mPrototype = function->getPrototype();
	visit(static_cast<tree::Scope *>(function));
	mPrototype = NULL;
}

void operation::TypeExpressions::visit(tree::If *ifStatement)
{
	LOG("TypeExpressions::setup::If");

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

void operation::TypeExpressions::visit(tree::While *whileStatement)
{
	LOG("TypeExpressions::visit::While");

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

void operation::TypeExpressions::visit(tree::Return *returnStatement)
{
	LOG("TypeExpressions::visit::Return");

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


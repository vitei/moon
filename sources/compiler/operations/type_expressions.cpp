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
		binaryExpression->setLHS(createCast(binaryExpression->getLocation(), binaryExpression->getType(), binaryExpression->getLHS()));
	}

	if(binaryExpression->getRHS() && *binaryExpression->getRHS()->getType() != *binaryExpression->getType())
	{
		binaryExpression->setRHS(createCast(binaryExpression->getLocation(), binaryExpression->getType(), binaryExpression->getRHS()));
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
			if(typeA->canCast(*typeB))
			{
				booleanBinaryExpression->setRHS(createCast(booleanBinaryExpression->getLocation(), typeA, booleanBinaryExpression->getRHS()));
			}
			else
			{
				booleanBinaryExpression->setLHS(createCast(booleanBinaryExpression->getLocation(), typeB, booleanBinaryExpression->getLHS()));
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
				*i = createCast((*i)->getLocation(), expectedType, *i);
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

		ifStatement->setTest(createCast(test->getLocation(), new tree::Bool(), test));
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

		whileStatement->setTest(createCast(test->getLocation(), new tree::Bool(), test));
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
		returnStatement->setReturn(createCast(returnStatement->getLocation(), mPrototype->getType(), returnStatement->getReturn()));
	} 
}

tree::Cast *operation::TypeExpressions::createCast(const tree::Node::Location &castLocation, tree::Type *type, tree::Expression *expression)
{
	try
	{
		return new tree::Cast(type, expression);
	}
	catch(tree::Cast::InvalidException &e)
	{
		if(dynamic_cast<tree::Void *>(expression->getType()))
		{
			ASSERT(dynamic_cast<tree::FunctionCall *>(expression));

			tree::FunctionCall *functionCall = static_cast<tree::FunctionCall *>(expression);

			ASSERT(dynamic_cast<tree::FunctionPrototype *>(functionCall->getPrototype()));

			std::string error = "Function \"" + static_cast<tree::FunctionPrototype *>(functionCall->getPrototype())->getName() + "\" does not return a value";

			error::enqueue(castLocation, error);
		}
		else
		{
			std::string error = "Cannot cast " + std::string(expression->getType()->getTypeName()) + " to " + std::string(type->getTypeName());

			error::enqueue(castLocation, error);
		}

		return NULL;
	}
}

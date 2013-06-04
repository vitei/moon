#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::TypeExpressions::run(tree::Program *program)
{
	operation::TypeExpressions operation;
	program->accept(&operation);
}

void operation::TypeExpressions::visit(tree::Access *access)
{
	LOG("TypeExpressions::visit::Access");

	if(!access->getType())
	{
		// Check in-case unresolved
		if(access->getTarget())
		{
			access->setType(access->getTarget()->getType());
		}
	}
}

void operation::TypeExpressions::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("TypeExpressions::visit::ArrayAccess");

	if(!arrayAccess->getType())
	{
		// Check in-case unresolved
		if(arrayAccess->getContainer())
		{
			arrayAccess->setType(arrayAccess->getContainer()->getType());
		}
	}
}

void operation::TypeExpressions::visit(tree::BinaryExpression *binaryExpression)
{
	LOG("TypeExpressions::visit::BinaryExpression");

	if(!binaryExpression->getType())
	{
		// Check in-case unresolved
		if(binaryExpression->getLHS() && binaryExpression->getRHS())
		{
			tree::Type *lhsType = binaryExpression->getLHS()->getType();
			tree::Type *rhsType = binaryExpression->getRHS()->getType();

			if(!lhsType)
			{
				lhsType = rhsType;
				binaryExpression->getLHS()->setType(lhsType);
			}

			ASSERT(lhsType);
			ASSERT(rhsType);

			if(*lhsType > *rhsType)
			{
				if(*lhsType != *rhsType)
				{
#ifdef DEBUG
					lhsType->printType();
					rhsType->printType();
#endif

					binaryExpression->setRHS(new tree::Cast(lhsType, binaryExpression->getRHS()));
				}

				binaryExpression->setType(lhsType);
			}
			else
			{
				if(*lhsType != *rhsType)
			{
#ifdef DEBUG
					lhsType->printType();
					rhsType->printType();
#endif

					binaryExpression->setLHS(new tree::Cast(rhsType, binaryExpression->getLHS()));
				}

				binaryExpression->setType(rhsType);
			}
		}
	}
}

void operation::TypeExpressions::visit(tree::Assign *assign)
{
	LOG("TypeExpressions::visit::Assign");

	if(!assign->getType())
	{
		// Check in-case unresolved
		if(assign->getLHS() && assign->getRHS())
		{
			tree::Type *lhsType = assign->getLHS()->getType();
			tree::Type *rhsType = assign->getRHS()->getType();

			if(!lhsType)
			{
				lhsType = rhsType;
				assign->getLHS()->setType(lhsType);
			}

			ASSERT(lhsType);
			ASSERT(rhsType);

			if(*lhsType != *rhsType)
			{
#ifdef DEBUG
				lhsType->printType();
				rhsType->printType();
#endif

				assign->setRHS(new tree::Cast(lhsType, assign->getRHS()));
			}

			assign->setType(lhsType);
		}
	}
}

void operation::TypeExpressions::visit(tree::BooleanBinaryExpression *booleanBinaryExpression)
{
	LOG("TypeExpressions::visit::BooleanBinaryExpression");

	tree::Type *booleanType = booleanBinaryExpression->getType();

	ASSERT(booleanType);
	ASSERT(dynamic_cast<tree::Bool *>(booleanType));

	booleanBinaryExpression->setType(NULL);
	operation::TypeExpressions::visit(static_cast<tree::BinaryExpression *>(booleanBinaryExpression));
	booleanBinaryExpression->setType(booleanType);
}

void operation::TypeExpressions::visit(tree::UnaryExpression *unaryExpression)
{
	LOG("TypeExpressions::visit::UnaryExpression");

	if(!unaryExpression->getType())
	{
		// Check in-case unresolved
		if(unaryExpression->getExpression())
		{
			unaryExpression->setType(unaryExpression->getExpression()->getType());
		}
	}
}

void operation::TypeExpressions::visit(tree::BooleanUnaryExpression *booleanUnaryExpression)
{
	LOG("TypeExpressions::visit::BooleanUnaryExpression");

	ASSERT(booleanUnaryExpression->getType());
	ASSERT(dynamic_cast<tree::Bool *>(booleanUnaryExpression->getType()));
}

void operation::TypeExpressions::visit(tree::FunctionCall *functionCall)
{
	LOG("TypeExpressions::visit::FunctionCall");

	if(!functionCall->getType())
	{
		// Check in-case unresolved
		if(functionCall->getPrototype())
		{
			tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

			functionCall->setType(functionPrototype->getType());

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

					//ASSERT(expectedType);
					//ASSERT(actualType);

					// Check the types in-case unresolved
					if((expectedType && actualType) && *expectedType != *actualType)
					{
#ifdef DEBUG
						actualType->printType();
						expectedType->printType();
#endif

						*i = new tree::Cast(expectedType, *i);
					}
				}
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

void operation::TypeExpressions::setup(tree::Function *function)
{
	LOG("TypeExpressions::setup::Function");

	mPrototype = function->getPrototype();
	mReturnType = mPrototype ? mPrototype->getType() : NULL;
}

void operation::TypeExpressions::visit(tree::Function *function)
{
	LOG("TypeExpressions::visit::Function");

	if(mPrototype && !mPrototype->getType())
	{
		mReturnType = NULL;
		visit(static_cast<tree::Scope *>(function));
		mPrototype->setType(mReturnType ? mReturnType : new tree::Void());
	}

	visit(static_cast<tree::Scope *>(function));
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

		ifStatement->setTest(new tree::Cast(new tree::Bool(), test));
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

		whileStatement->setTest(new tree::Cast(new tree::Bool(), test));
	}
}

void operation::TypeExpressions::visit(tree::Return *returnStatement)
{
	// Only bother with this if there is a prototype...
	if(mPrototype)
	{
		if(!mPrototype->getType())
		{
			if(!mReturnType || returnStatement->getReturn()->getType() < mReturnType)
			{
				mReturnType = returnStatement->getReturn()->getType();
			}
		}
		else if(returnStatement->getReturn()->getType() != mPrototype->getType())
		{
			returnStatement->setReturn(new tree::Cast(mPrototype->getType(), returnStatement->getReturn()));
		}
	}
}

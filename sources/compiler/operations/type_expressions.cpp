#include "compiler/tree.h"
#include "compiler/operations/type_expressions.h"


void operation::TypeExpressions::run(tree::Program *program)
{
	operation::TypeExpressions operation;
	program->accept(&operation);
}

void operation::TypeExpressions::visit(tree::Access *access)
{
	LOG("TypeExpressions::visit::Access");

	ASSERT(!access->getType());

	// Check in-case unresolved
	if(access->getTarget())
	{
		access->setType(access->getTarget()->getType());
	}
}

void operation::TypeExpressions::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("TypeExpressions::visit::ArrayAccess");

	ASSERT(!arrayAccess->getType());

	// Check in-case unresolved
	if(arrayAccess->getContainer())
	{
		arrayAccess->setType(arrayAccess->getContainer()->getType());
	}
}

void operation::TypeExpressions::visit(tree::BinaryExpression *binaryExpression)
{
	LOG("TypeExpressions::visit::BinaryExpression");

	ASSERT(!binaryExpression->getType());

	// Check in-case unresolved
	if(binaryExpression->getLHS() && binaryExpression->getRHS())
	{
		tree::Type *lhsType = binaryExpression->getLHS()->getType();
		tree::Type *rhsType = binaryExpression->getRHS()->getType();

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
}

void operation::TypeExpressions::visit(tree::UnaryExpression *unaryExpression)
{
	LOG("TypeExpressions::visit::UnaryExpression");

	ASSERT(!unaryExpression->getType());

	// Check in-case unresolved
	if(unaryExpression->getExpression())
	{
		unaryExpression->setType(unaryExpression->getExpression()->getType());
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

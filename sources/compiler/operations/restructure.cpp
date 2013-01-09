#include "compiler/tree.h"
#include "compiler/operations/restructure.h"


void operation::Restructure::visit(tree::Node *node)
{
	mNodeMap.push(node->restructure(this));
}

void operation::Restructure::visit(tree::Expression *expression)
{
	if(expression->getType())
	{
		tree::Type *type = static_cast<tree::Type *>(mNodeMap.top());
		mNodeMap.pop();

		expression->setType(type);
	}

	mNodeMap.push(expression->restructure(this));
}

void operation::Restructure::visit(tree::Access *access)
{
	if(access->getTarget())
	{
		tree::Expression *target = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		access->setTarget(target);
	}

	if(access->getContainer())
	{
		tree::Expression *container = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		access->setContainer(container);
	}

	mNodeMap.push(access->restructure(this));
}

void operation::Restructure::visit(tree::UnaryExpression *unaryExpression)
{
	if(unaryExpression->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		unaryExpression->setExpression(expression);
	}

	mNodeMap.push(unaryExpression->restructure(this));
}

void operation::Restructure::visit(tree::BinaryExpression *binaryExpression)
{
	if(binaryExpression->getRHS())
	{
		tree::Expression *rhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		binaryExpression->setRHS(rhs);
	}

	if(binaryExpression->getLHS())
	{
		tree::Expression *lhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		binaryExpression->setLHS(lhs);
	}

	mNodeMap.push(binaryExpression->restructure(this));
}

void operation::Restructure::visit(tree::Cast *cast)
{
	if(cast->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		cast->setExpression(expression);
	}

	mNodeMap.push(cast->restructure(this));
}

void operation::Restructure::visit(tree::FunctionCall *functionCall)
{
	if(functionCall->getFunction())
	{
		tree::Expression *function = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		functionCall->setFunction(function);
	}

	tree::Expressions *expressions = functionCall->getArguments();

	if(expressions)
	{
		for(tree::Expressions::iterator i = expressions->begin(); i != expressions->end();)
		{
			(*i)->accept(this);

			tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
			mNodeMap.pop();

			if(expression)
			{
				*i = expression;
				++i;
			}
			else
			{
				i = expressions->erase(i);
			}
		}
	}

	mNodeMap.push(functionCall->restructure(this));
}

void operation::Restructure::visit(tree::Scoping *scoping)
{
	if(scoping->getScoped())
	{
		tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();

		scoping->setScoped(statement);
	}

	mNodeMap.push(scoping->restructure(this));
}

void operation::Restructure::visit(tree::Execute *execute)
{
	if(execute->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		execute->setExpression(expression);
	}

	mNodeMap.push(execute->restructure(this));
}

void operation::Restructure::visit(tree::Return *opReturn)
{
	if(opReturn->getReturn())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		opReturn->setReturn(expression);
	}

	mNodeMap.push(opReturn->restructure(this));
}

void operation::Restructure::visit(tree::SetState *setState)
{
	if(setState->getState())
	{
		tree::State *state = static_cast<tree::State *>(mNodeMap.top());
		mNodeMap.pop();

		setState->setState(state);
	}

	mNodeMap.push(setState->restructure(this));
}

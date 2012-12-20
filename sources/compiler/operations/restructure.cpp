#include "compiler/tree.h"
#include "compiler/operations/restructure.h"


void operation::Restructure::visit(tree::Node *node)
{
	mNodeMap.push(node->restructure(this));
}

void operation::Restructure::visit(tree::TypedIdentity *typedIdentity)
{
	tree::Type *type = static_cast<tree::Type *>(mNodeMap.top());
	mNodeMap.pop();

	typedIdentity->setType(type);
	mNodeMap.push(typedIdentity->restructure(this));
}

void operation::Restructure::visit(tree::Access *access)
{
	tree::Expression *target = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();
	tree::Expression *container = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();

	access->setContainer(container);
	access->setTarget(target);
	mNodeMap.push(access->restructure(this));
}

void operation::Restructure::visit(tree::Literal *literal)
{
	tree::Type *type = static_cast<tree::Type *>(mNodeMap.top());
	mNodeMap.pop();

	literal->setType(type);
	mNodeMap.push(literal->restructure(this));
}

void operation::Restructure::visit(tree::UnaryExpression *unaryExpression)
{
	tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();

	unaryExpression->setExpression(expression);
	mNodeMap.push(unaryExpression->restructure(this));
}

void operation::Restructure::visit(tree::BinaryExpression *binaryExpression)
{
	tree::Expression *rhs = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();
	tree::Expression *lhs = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();

	binaryExpression->setLHS(lhs);
	binaryExpression->setRHS(rhs);
	mNodeMap.push(binaryExpression->restructure(this));
}

void operation::Restructure::visit(tree::Cast *cast)
{
	tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();
	tree::Type *type = static_cast<tree::Type *>(mNodeMap.top());
	mNodeMap.pop();

	cast->setType(type);
	cast->setExpression(expression);
	mNodeMap.push(cast->restructure(this));
}

void operation::Restructure::visit(tree::FunctionCall *functionCall)
{
	tree::Identifier *id = static_cast<tree::Identifier *>(mNodeMap.top());
	mNodeMap.pop();

	functionCall->setID(id);

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

void operation::Restructure::visit(tree::FunctionPrototype *functionPrototype)
{
	tree::Expressions *expressions = functionPrototype->getArguments();

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

	mNodeMap.push(functionPrototype->restructure(this));
}



void operation::Restructure::visit(tree::Scoping *scoping)
{
	tree::Identity *identity = static_cast<tree::Identity *>(mNodeMap.top());
	mNodeMap.pop();

	scoping->setScoped(identity);
	mNodeMap.push(scoping->restructure(this));
}

void operation::Restructure::visit(tree::Execute *execute)
{
	tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();

	execute->setExpression(expression);
	mNodeMap.push(execute->restructure(this));
}

void operation::Restructure::visit(tree::Return *opReturn)
{
	tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
	mNodeMap.pop();

	opReturn->setReturn(expression);
	mNodeMap.push(opReturn->restructure(this));
}

void operation::Restructure::visit(tree::SetState *setState)
{
	tree::State *state = static_cast<tree::State *>(mNodeMap.top());
	mNodeMap.pop();

	setState->setState(state);
	mNodeMap.push(setState->restructure(this));
}

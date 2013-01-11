#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::Restructure::visit(tree::Node *node)
{
	GENERATE_DISPATCH(node, dispatch)
}

void operation::Restructure::dispatch(tree::Node *node)
{
	mNodeMap.push(node->restructure(this));
}

void operation::Restructure::dispatch(tree::Expression *expression)
{
	tree::Type *type = NULL;

	if(expression->getType())
	{
		type = static_cast<tree::Type *>(mNodeMap.top());
		mNodeMap.pop();
	}

	expression->setType(type);

	mNodeMap.push(expression->restructure(this));
}

void operation::Restructure::dispatch(tree::Access *access)
{
	tree::Expression *container = NULL;
	tree::Expression *target = NULL;

	if(access->getTarget())
	{
		target = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(access->getContainer())
	{
		container = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	access->setContainer(container);
	access->setTarget(target);

	mNodeMap.push(access->restructure(this));
}

void operation::Restructure::dispatch(tree::UnaryExpression *unaryExpression)
{
	tree::Expression *expression = NULL;

	if(unaryExpression->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	unaryExpression->setExpression(expression);

	mNodeMap.push(unaryExpression->restructure(this));
}

void operation::Restructure::dispatch(tree::BinaryExpression *binaryExpression)
{
	tree::Expression *lhs = NULL;
	tree::Expression *rhs = NULL;

	if(binaryExpression->getRHS())
	{
		rhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(binaryExpression->getLHS())
	{
		lhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	binaryExpression->setLHS(lhs);
	binaryExpression->setRHS(rhs);

	mNodeMap.push(binaryExpression->restructure(this));
}

void operation::Restructure::dispatch(tree::Cast *cast)
{
	tree::Expression *expression = NULL;

	if(cast->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	cast->setExpression(expression);

	mNodeMap.push(cast->restructure(this));
}

void operation::Restructure::dispatch(tree::FunctionCall *functionCall)
{
	tree::Expression *functionPrototype = NULL;

	if(functionCall->getPrototype())
	{
		functionPrototype = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	functionCall->setFunctionPrototype(functionPrototype);

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

void operation::Restructure::dispatch(tree::Scoping *scoping)
{
	tree::Statement *statement = NULL;

	if(scoping->getScoped())
	{
		statement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();
	}

	scoping->setScoped(statement);

	mNodeMap.push(scoping->restructure(this));
}

void operation::Restructure::dispatch(tree::Execute *execute)
{
	tree::Expression *expression = NULL;

	if(execute->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	execute->setExpression(expression);

	mNodeMap.push(execute->restructure(this));
}

void operation::Restructure::dispatch(tree::Return *opReturn)
{
	tree::Expression *expression = NULL;

	if(opReturn->getReturn())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	opReturn->setReturn(expression);

	mNodeMap.push(opReturn->restructure(this));
}

void operation::Restructure::dispatch(tree::SetState *setState)
{
	tree::State *state = NULL;

	if(setState->getState())
	{
		state = static_cast<tree::State *>(mNodeMap.top());
		mNodeMap.pop();
	}

	setState->setState(state);

	mNodeMap.push(setState->restructure(this));
}

void operation::Restructure::dispatch(tree::FunctionPrototype *functionPrototype)
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

void operation::Restructure::dispatch(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
	mNodeMap.pop();

	function->setPrototype(functionPrototype);

	tree::Statements *statements = function->getStatements();

	if(statements)
	{
		tree::Scope *currentScope = mCurrentScope;

		mCurrentScope = function;

		for(tree::Statements::iterator i = statements->begin(); i != statements->end();)
		{
			(*i)->accept(this);

			tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
			mNodeMap.pop();

			if(statement)
			{
				*i = statement;
				++i;
			}
			else
			{
				i = statements->erase(i);
			}
		}

		mCurrentScope = currentScope;
	}

	mNodeMap.push(function->restructure(this));
}

void operation::Restructure::dispatch(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		tree::Scope *currentScope = mCurrentScope;

		mCurrentScope = scope;

		for(tree::Statements::iterator i = statements->begin(); i != statements->end();)
		{
			(*i)->accept(this);

			tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
			mNodeMap.pop();

			if(statement)
			{
				*i = statement;
				++i;
			}
			else
			{
				i = statements->erase(i);
			}
		}

		mCurrentScope = currentScope;
	}

	mNodeMap.push(scope->restructure(this));
}

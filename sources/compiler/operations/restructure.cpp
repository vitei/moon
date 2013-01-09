#include "compiler/tree.h"
#include "compiler/operations/restructure.h"


void operation::Restructure::visit(tree::Node *node)
{
	GENERATE_DISPATCH(node, doRestructure)
}

void operation::Restructure::doRestructure(tree::Node *node)
{
	mNodeMap.push(node->restructure(this));
}

void operation::Restructure::doRestructure(tree::Expression *expression)
{
	if(expression->getType())
	{
		tree::Type *type = static_cast<tree::Type *>(mNodeMap.top());
		mNodeMap.pop();

		expression->setType(type);
	}

	mNodeMap.push(expression->restructure(this));
}

void operation::Restructure::doRestructure(tree::Access *access)
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

void operation::Restructure::doRestructure(tree::UnaryExpression *unaryExpression)
{
	if(unaryExpression->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		unaryExpression->setExpression(expression);
	}

	mNodeMap.push(unaryExpression->restructure(this));
}

void operation::Restructure::doRestructure(tree::BinaryExpression *binaryExpression)
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

void operation::Restructure::doRestructure(tree::Cast *cast)
{
	if(cast->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		cast->setExpression(expression);
	}

	mNodeMap.push(cast->restructure(this));
}

void operation::Restructure::doRestructure(tree::FunctionCall *functionCall)
{
	if(functionCall->getFunctionPrototype())
	{
		tree::Expression *functionPrototype = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		functionCall->setFunctionPrototype(functionPrototype);
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

void operation::Restructure::doRestructure(tree::Scoping *scoping)
{
	if(scoping->getScoped())
	{
		tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();

		scoping->setScoped(statement);
	}

	mNodeMap.push(scoping->restructure(this));
}

void operation::Restructure::doRestructure(tree::Execute *execute)
{
	if(execute->getExpression())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		execute->setExpression(expression);
	}

	mNodeMap.push(execute->restructure(this));
}

void operation::Restructure::doRestructure(tree::Return *opReturn)
{
	if(opReturn->getReturn())
	{
		tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();

		opReturn->setReturn(expression);
	}

	mNodeMap.push(opReturn->restructure(this));
}

void operation::Restructure::doRestructure(tree::SetState *setState)
{
	if(setState->getState())
	{
		tree::State *state = static_cast<tree::State *>(mNodeMap.top());
		mNodeMap.pop();

		setState->setState(state);
	}

	mNodeMap.push(setState->restructure(this));
}

void operation::Restructure::doRestructure(tree::FunctionPrototype *functionPrototype)
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

void operation::Restructure::doRestructure(tree::Function *function)
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

void operation::Restructure::doRestructure(tree::Scope *scope)
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

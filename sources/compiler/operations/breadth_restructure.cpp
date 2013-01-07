#include "compiler/tree.h"
#include "compiler/operations/breadth_restructure.h"


void operation::BreadthRestructure::add(tree::Scope *scope)
{
	mVisitNext.push(scope);
}

void operation::BreadthRestructure::process()
{
	while(!mVisitNext.empty())
	{
		tree::Scope *scope = mVisitNext.front();
		tree::Statements *statements = scope->getStatements();

		mCurrentScope = scope;
		beginScope(scope);

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

		mVisitNext.pop();
	}
}

void operation::BreadthRestructure::visit(tree::FunctionPrototype *functionPrototype)
{
	mNodeMap.push(functionPrototype->restructure(this));
}

void operation::BreadthRestructure::visit(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
	mNodeMap.pop();

	function->setPrototype(functionPrototype);

	if(function->getStatements())
	{
		add(function);
	}

	tree::Expressions *expressions = functionPrototype->getArguments();

	if(expressions)
	{
		tree::Scope *currentScope = mCurrentScope;

		mCurrentScope = function;
		beginScope(function);

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

		mCurrentScope = currentScope;
		beginScope(currentScope);
	}

	mNodeMap.push(function->restructure(this));
}

void operation::BreadthRestructure::visit(tree::Scope *scope)
{
	if(scope->getStatements())
	{
		add(scope);
	}

	mNodeMap.push(scope->restructure(this));
}

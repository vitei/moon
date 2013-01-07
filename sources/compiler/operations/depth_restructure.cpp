#include "compiler/tree.h"
#include "compiler/operations/depth_restructure.h"


void operation::DepthRestructure::visit(tree::FunctionPrototype *functionPrototype)
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

void operation::DepthRestructure::visit(tree::Function *function)
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

void operation::DepthRestructure::visit(tree::Scope *scope)
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

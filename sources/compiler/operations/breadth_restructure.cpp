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

		beginScope(scope);

		for(tree::Statements::iterator i = statements->begin(), end = statements->end(); i != end; ++i)
		{
			(*i)->accept(this);

			tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
			mNodeMap.pop();

			*i = statement;
		}

		mVisitNext.pop();
	}
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

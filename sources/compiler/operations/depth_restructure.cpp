#include "compiler/tree.h"
#include "compiler/operations/depth_restructure.h"


void operation::DepthRestructure::visit(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
	mNodeMap.pop();

	function->setPrototype(functionPrototype);

	tree::Statements *statements = function->getStatements();

	if(statements)
	{
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
	}

	mNodeMap.push(function->restructure(this));
}

void operation::DepthRestructure::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
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
	}

	mNodeMap.push(scope->restructure(this));
}

#include "common.h"

#include "compiler/generators/c/output_new.h"

void generator::c::OutputNew::run(generator::c::Printer *printer, tree::Program *program)
{
	generator::c::OutputNew operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::c::OutputNew::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); ++i)
		{
			if(!dynamic_cast<tree::Function *>(*i))
			{
				(*i)->accept(this);
			}
		}
	}
}

void generator::c::OutputNew::visit(tree::Program *program)
{
	mPrinter->outputNewBegin();
	visit(static_cast<tree::Scope *>(program));
	mPrinter->outputNewEnd();
}

void generator::c::OutputNew::visit(tree::Aggregate *aggregate)
{
	outputScope(aggregate);
	visit(static_cast<tree::Scope *>(aggregate));
}

void generator::c::OutputNew::visit(tree::Use *use)
{
	outputScope(use);
}

void generator::c::OutputNew::outputScope(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); ++i)
		{
			// Only assign expressions should be output here
			tree::Execute *execute = dynamic_cast<tree::Execute *>(*i);

			if(execute && dynamic_cast<tree::Assign *>(execute->getExpression()))
			{
				mPrinter->dispatch(execute);
			}
		}
	}
}


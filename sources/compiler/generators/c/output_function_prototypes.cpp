#include "common.h"

#include "compiler/generators/c/output_function_prototypes.h"

void generator::c::OutputFunctionPrototypes::run(generator::c::Printer *printer, tree::Program *program)
{
	generator::c::OutputFunctionPrototypes operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::c::OutputFunctionPrototypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}
}

void generator::c::OutputFunctionPrototypes::visit(tree::Function *function)
{
	tree::FunctionPrototype *prototype = function->getPrototype();

	if(mPrototypesOutput.find(prototype) == mPrototypesOutput.end())
	{
		mPrototypesOutput.insert(prototype);
		mPrinter->output(prototype);
		mPrinter->outputEOS();
	}
}

void generator::c::OutputFunctionPrototypes::visit(tree::Import *import)
{
	tree::FunctionPrototype *prototype = import->getPrototype();

	if(mPrototypesOutput.find(prototype) == mPrototypesOutput.end())
	{
		mPrototypesOutput.insert(prototype);
		mPrinter->output(import);
		mPrinter->outputEOS();
	}
}


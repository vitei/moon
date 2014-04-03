#include "common.h"

#include "compiler/generators/obj_c/output_function_prototypes.h"

void generator::objC::OutputFunctionPrototypes::run(generator::objC::Printer *printer, tree::Program *program)
{
	generator::objC::OutputFunctionPrototypes operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::objC::OutputFunctionPrototypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}
}

void generator::objC::OutputFunctionPrototypes::visit(tree::Function *function)
{
	tree::FunctionPrototype *prototype = function->getPrototype();

	if(mPrototypesOutput.find(prototype) == mPrototypesOutput.end())
	{
		mPrototypesOutput.insert(prototype);
		mPrinter->output(prototype);
		mPrinter->outputEOS();
	}
}

void generator::objC::OutputFunctionPrototypes::visit(tree::Import *import)
{
	tree::FunctionPrototype *prototype = import->getPrototype();

	if(mPrototypesOutput.find(prototype) == mPrototypesOutput.end())
	{
		mPrototypesOutput.insert(prototype);
		mPrinter->output(import);
		mPrinter->outputEOS();
	}
}


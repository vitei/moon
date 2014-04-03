#include "common.h"

#include "compiler/generators/obj_c/output_functions.h"

void generator::objC::OutputFunctions::run(generator::objC::Printer *printer, tree::Program *program)
{
	generator::objC::OutputFunctions operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::objC::OutputFunctions::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void generator::objC::OutputFunctions::visit(tree::Function *function)
{
	mPrinter->output(function);
}


void generator::objC::OutputFunctions::visit(tree::Method *method)
{
	mPrinter->output(method);
}


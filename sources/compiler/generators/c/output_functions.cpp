#include "common.h"

#include "compiler/generators/c/output_functions.h"

void generator::c::OutputFunctions::run(generator::c::Printer *printer, tree::Program *program)
{
	generator::c::OutputFunctions operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::c::OutputFunctions::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void generator::c::OutputFunctions::visit(tree::Function *function)
{
	mPrinter->output(function);
}


void generator::c::OutputFunctions::visit(tree::Method *method)
{
	mPrinter->output(method);
}


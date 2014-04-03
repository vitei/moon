#include "common.h"

#include "compiler/generators/c/output_types.h"

void generator::c::OutputTypes::run(generator::c::Printer *printer, tree::Program *program)
{
	generator::c::OutputTypes operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::c::OutputTypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}
}

void generator::c::OutputTypes::visit(tree::UDT *udt)
{
	tree::Members *members = udt->getMembers();

	for(tree::Members::iterator i = members->begin(); i != members->end(); (*i++)->accept(this))
		;

	if(mTypesOutput.find(udt) == mTypesOutput.end())
	{
		mTypesOutput.insert(udt);
		mPrinter->output(udt);
	}
}


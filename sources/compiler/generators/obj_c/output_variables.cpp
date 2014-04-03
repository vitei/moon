#include "common.h"

#include "compiler/generators/obj_c/output_variables.h"

void generator::objC::OutputVariables::run(generator::objC::Printer *printer, tree::Program *program)
{
	generator::objC::OutputVariables operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void generator::objC::OutputVariables::visit(tree::Program *program)
{
	for(tree::Scope::NamedNodes::iterator i = program->getNamedNodes().begin(), end = program->getNamedNodes().end(); i != end; ++i)
	{
		if(dynamic_cast<tree::Variable *>(i->second))
		{
			mPrinter->outputExtern(static_cast<tree::TypedIdentity *>(i->second));
		}
	}

	visitScope(program);

	mPrinter->outputVariablesBegin();

	for(std::list<tree::TypedIdentity *>::iterator i = mIdentities.begin(), e = mIdentities.end(); i != e; ++i)
	{
		mPrinter->outputTabs();
		mPrinter->outputDeclaration(*i);
		mPrinter->outputEOS();
	}

	mPrinter->outputVariablesEnd();
}

void generator::objC::OutputVariables::visit(tree::Aggregate *aggregate)
{
	for(tree::Scope::NamedNodes::iterator i = aggregate->getNamedNodes().begin(), end = aggregate->getNamedNodes().end(); i != end; ++i)
	{
		if(dynamic_cast<tree::Variable *>(i->second))
		{
			mIdentities.push_back(static_cast<tree::TypedIdentity *>(i->second));
		}
	}

	visitScope(aggregate);
}

void generator::objC::OutputVariables::visit(tree::Use *use)
{
	for(tree::Scope::NamedNodes::iterator i = use->getNamedNodes().begin(), end = use->getNamedNodes().end(); i != end; ++i)
	{
		if(dynamic_cast<tree::Variable *>(i->second))
		{
			mIdentities.push_back(static_cast<tree::TypedIdentity *>(i->second));
		}
	}
}

void generator::objC::OutputVariables::visitScope(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}


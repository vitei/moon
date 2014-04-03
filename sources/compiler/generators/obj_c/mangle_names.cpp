#include "common.h"

#include "compiler/generators/obj_c/mangled.h"
#include "compiler/generators/obj_c/mangle_names.h"

void generator::objC::MangleNames::run(tree::Program *program)
{
	MangleNames operation;
	program->accept(&operation);
}

void generator::objC::MangleNames::visit(tree::Scope *scope)
{
	behaviour::NamedMap::AssociatedNamedNodes associatedNamedNodes = scope->getAssociatedNamedNodes();

	for(tree::Scope::AssociatedNamedNodes::iterator i = associatedNamedNodes.begin(), end = associatedNamedNodes.end(); i != end; ++i)
	{
		behaviour::NamedMap::NamedNodes namedNodes = i->second;

		for(tree::Scope::NamedNodes::iterator j = namedNodes.begin(), end = namedNodes.end(); j != end; ++j)
		{
			dispatch(j->second);
		}
	}

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}
}

void generator::objC::MangleNames::visit(tree::Program *program)
{
	tree::Program *oldProgram = mProgram;

	mProgram = program;
	visit(static_cast<tree::Scope *>(program));
	mProgram = oldProgram;
}

void generator::objC::MangleNames::visit(tree::Aggregate *aggregate)
{
	tree::Aggregate *oldAggregate = mAggregate;

	mAggregate = aggregate;
	visit(static_cast<tree::Scope *>(aggregate));
	mAggregate = oldAggregate;
}

void generator::objC::MangleNames::visit(tree::Use *use)
{
	tree::Use *oldUse = mUse;

	mUse = use;
	visit(static_cast<tree::Scope *>(use));
	mUse = oldUse;
}

void generator::objC::MangleNames::visit(tree::Function *function)
{
	tree::Function *oldFunction = mFunction;

	mFunction = function;

	behaviour::NamedMap::AssociatedNamedNodes associatedNamedNodes = function->getAssociatedNamedNodes();
	tree::Expressions *arguments = function->getPrototype()->getArguments();

	for(tree::Scope::AssociatedNamedNodes::iterator i = associatedNamedNodes.begin(), end = associatedNamedNodes.end(); i != end; ++i)
	{
		behaviour::NamedMap::NamedNodes namedNodes = i->second;

		for(tree::Scope::NamedNodes::iterator j = namedNodes.begin(), end = namedNodes.end(); j != end; ++j)
		{
			// If this is a parameter then it will have been processed already!
			// FIXME, this is probably not that efficient...
			if(!arguments || std::find(arguments->begin(), arguments->end(), j->second) == arguments->end())
			{
				dispatch(j->second);
			}
		}
	}

	tree::Statements *statements = function->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}

	mFunction = oldFunction;
}

void generator::objC::MangleNames::visit(tree::Method *method)
{
	tree::Type *oldType = mType;

	mType = method->getType();
	visit(static_cast<tree::Function *>(method));
	mType = oldType;
}

void generator::objC::MangleNames::dispatch(tree::Node *node)
{
	GENERATE_DISPATCH(node, mangle)
}

void generator::objC::MangleNames::mangle(tree::Identity *identity)
{
	ASSERT(!identity->getMetadata());

	std::string mangledName = "moon";

	ASSERT(mProgram);
	mangledName += "$$";
	mangledName += mProgram->getName();

	if(mUse)
	{
		mangledName += "$$";
		mangledName += mUse->getName();
	}

	if(mType)
	{
		mangledName += "$$";
		mangledName += mType->getTypeName();
	}

	if(mFunction)
	{
		mangledName += "$$";
		mangledName += mFunction->getPrototype()->getName();
	}

	mangledName += "$$";
	mangledName += identity->getName();

	identity->setMetadata(new generator::objC::Mangled(mangledName));
}

void generator::objC::MangleNames::mangle(tree::Variable *variable)
{
	mangle(static_cast<tree::Identity *>(variable));

	if(mUse && !mFunction)
	{
		generator::objC::Mangled *cName = static_cast<generator::objC::Mangled *>(variable->getMetadata());
		cName->useName = "moon$$scope->" + cName->declarationName;
	}
}

void generator::objC::MangleNames::mangle(tree::FunctionPrototype *functionPrototype)
{
	tree::Function *function = tree::node_cast<tree::Function *>(functionPrototype->getTarget());

	if(function)
	{
		tree::Method *method = tree::node_cast<tree::Method *>(function);

		if(method)
		{
			tree::Type *oldType = mType;

			mType = method->getType();
			mangle(static_cast<tree::Identity *>(functionPrototype));
			mType = oldType;
		}
		else
		{
			mangle(static_cast<tree::Identity *>(functionPrototype));
		}
	}
	else
	{
		ASSERT(tree::node_cast<tree::Import *>(functionPrototype->getTarget()));
		functionPrototype->setMetadata(new generator::objC::Mangled(functionPrototype->getName(), true));
	}

	tree::Expressions *arguments = functionPrototype->getArguments();

	if(arguments)
	{
		tree::Function *oldFunction = mFunction;

		mFunction = function;

		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			dispatch(*i);
		}

		mFunction = oldFunction;
	}
}

void generator::objC::MangleNames::mangle(tree::UDT *udt)
{
	ASSERT(!udt->getMetadata());

	std::string mangledName = "moon";

	ASSERT(mProgram);
	mangledName += "$$";
	mangledName += mProgram->getName();

	if(mUse)
	{
		mangledName += "$$";
		mangledName += mUse->getName();
	}

	mangledName += "$$";
	mangledName += udt->getTypeName();

	udt->setMetadata(new generator::objC::Mangled(mangledName));

	for(tree::Members::iterator i = udt->getMembers()->begin(), end = udt->getMembers()->end(); i != end; ++i)
	{
		tree::Member *member = *i;

		ASSERT(!member->getMetadata());
		member->setMetadata(new generator::objC::Mangled("moon$$" + member->getName()));
	}
}


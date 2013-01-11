#include "compiler/tree.h"
#include "compiler/generators.h"


void generator::C::run(tree::Program *program)
{
	program->accept(this);

	LOG("\n\n%s", mOutput.str().c_str());
}

void generator::C::visit(tree::Program *program)
{
	mOutput << "typedef struct" << std::endl
		<< "{" << std::endl;

	for(tree::Identities::iterator i = program->getIdentities().begin(), end = program->getIdentities().end(); i != end; ++i)
	{
		mOutput << "\t";
		outputDeclaration(i->second);
		mOutput << ";" << std::endl;
	}

	mOutput << "} XXXX;" << std::endl;
}

void generator::C::outputDeclaration(tree::Identity *identity)
{
	bool isReference = dynamic_cast<tree::Reference *>(identity) != NULL;

	tree::Type *type = identity->getType();
	tree::Bool *boolean;
	tree::Int *integer;
	tree::Float *floatingPoint;
	tree::String *string;
	tree::UDT *udt;

	if((boolean = dynamic_cast<tree::Bool *>(type)))
	{
		if(isReference)
		{
			mOutput << "bool *" << identity->getName();
		}
		else
		{
			mOutput << "bool " << identity->getName();
		}
	}
	else if((integer = dynamic_cast<tree::Int *>(type)))
	{
		if(isReference)
		{
			mOutput << "int *" << identity->getName();
		}
		else
		{
			mOutput << "int " << identity->getName();
		}
	}
	else if((floatingPoint = dynamic_cast<tree::Float *>(type)))
	{
		if(isReference)
		{
			mOutput << "float *" << identity->getName();
		}
		else
		{
			mOutput << "float " << identity->getName();
		}
	}
	else if((string = dynamic_cast<tree::String *>(type)))
	{
		if(isReference)
		{
			mOutput << "char **" << identity->getName();
		}
		else
		{
			mOutput << "char " << identity->getName() << "[" << string->getMaxSize() << "]";
		}
	}
	else if((udt = dynamic_cast<tree::UDT *>(type)))
	{
		ERROR("FIXME");
		//mOutput << "char*";
	}
	else
	{
		ERROR("Unknown type");
	}
}

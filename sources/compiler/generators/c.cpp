#include "compiler/tree.h"
#include "compiler/generators.h"


void generator::C::run(std::ostream &output, tree::Program *program)
{
	mOutput = &output;
	dispatch(program);
}

void generator::C::dispatch(tree::Node *node)
{
	GENERATE_DISPATCH(node, generate)
}

void generator::C::generate(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); dispatch(*i++));
	}
}

void generator::C::generate(tree::Program *program)
{
	for(tree::Identities::iterator i = program->getIdentities().begin(), end = program->getIdentities().end(); i != end; ++i)
	{
		*mOutput << "extern ";
		outputDeclaration(i->second);
		*mOutput << ";" << std::endl;
	}

	/**mOutput << "typedef struct" << std::endl
		<< "{" << std::endl;

	for(tree::Identities::iterator i = program->getIdentities().begin(), end = program->getIdentities().end(); i != end; ++i)
	{
		outputDeclaration(i->second);
		*mOutput << ";" << std::endl;
	}

	*mOutput << "} FIXME_GENERATE_NAME;" << std::endl << std::endl;*/

	generate(static_cast<tree::Scope *>(program));
}

void generator::C::generate(tree::Function *function)
{
	outputDeclaration(function->getPrototype());
	*mOutput << "(FIXME_GENERATE_NAME *scope";

	tree::Expressions *arguments = function->getPrototype()->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			*mOutput << ", ";
			outputDeclaration(static_cast<tree::Identity *>(*i));
		}
	}

	*mOutput << ")" << std::endl
		<< "{" << std::endl;

	for(tree::Identities::iterator i = function->getIdentities().begin(), end = function->getIdentities().end(); i != end; ++i)
	{
		outputDeclaration(i->second);
		*mOutput << ";" << std::endl;
	}

	generate(static_cast<tree::Scope *>(function));

	tree::Statements *statements = function->getStatements();

	// Check if we need a return statement
	if(statements && dynamic_cast<tree::Return *>(*(--statements->end())) == NULL)
	{
		*mOutput << "return 0;" << std::endl;
	}

	*mOutput << "}" << std::endl << std::endl;
}

void generator::C::generate(tree::Identity *identity)
{
	*mOutput << identity->getName();
}

void generator::C::generate(tree::Reference *reference)
{
	*mOutput << "*" << reference->getName();
}

void generator::C::generate(tree::Cast *cast)
{
	tree::Type *type = cast->getType();

	if(dynamic_cast<tree::Bool *>(type))
	{
		*mOutput << "(bool)";
	}
	else if(dynamic_cast<tree::Int *>(type))
	{
		*mOutput << "(int)";
	}
	else if(dynamic_cast<tree::Float *>(type))
	{
		*mOutput << "(float)";
	}
	else if(dynamic_cast<tree::String *>(type))
	{
		*mOutput << "(char *)";
	}
	else if(dynamic_cast<tree::UDT *>(type))
	{
		ERROR("FIXME");
		//*mOutput << "char*";
	}
	else
	{
		ERROR("Unknown type");
	}

	*mOutput << "(";
	dispatch(cast->getExpression());
	*mOutput << ")";
}

void generator::C::generate(tree::DirectAccess *directAccess)
{
	// FIXME
}

void generator::C::generate(tree::MessageAccess *messageAccess)
{
	// FIXME
}

void generator::C::generate(tree::ArrayAccess *arrayAccess)
{
	// FIXME
}

void generator::C::generate(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *prototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

	*mOutput << prototype->getName() << "(scope";

	tree::Expressions *arguments = prototype->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			*mOutput << ", ";
			dispatch(*i);
		}
	}

	*mOutput << ")";
}

void generator::C::generate(tree::NullReference *nullReference)
{
	*mOutput << "NULL";
}

void generator::C::generate(tree::BoolLiteral *boolLiteral)
{
	*mOutput << (boolLiteral->getValue() ? "true" : "false");
}

void generator::C::generate(tree::IntLiteral *intLiteral)
{
	*mOutput << intLiteral->getValue();
}

void generator::C::generate(tree::FloatLiteral *floatLiteral)
{
	*mOutput << floatLiteral->getValue();
}

void generator::C::generate(tree::StringLiteral *stringLiteral)
{
	*mOutput << "\"" << stringLiteral->getValue() << "\"";
}

void generator::C::generate(tree::Assign *assign)
{
	dispatch(assign->getLHS());
	*mOutput << " = ";
	dispatch(assign->getRHS());
}

void generator::C::generate(tree::LogicalOr *logicalOr)
{
	*mOutput << "(";
	dispatch(logicalOr->getLHS());
	*mOutput << ") || (";
	dispatch(logicalOr->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::LogicalAnd *logicalAnd)
{
	*mOutput << "(";
	dispatch(logicalAnd->getLHS());
	*mOutput << ") && (";
	dispatch(logicalAnd->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Or *orExpression)
{
	*mOutput << "(";
	dispatch(orExpression->getLHS());
	*mOutput << ") | (";
	dispatch(orExpression->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Xor *xorExpression)
{
	*mOutput << "(";
	dispatch(xorExpression->getLHS());
	*mOutput << ") ^ (";
	dispatch(xorExpression->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::And *andExpression)
{
	*mOutput << "(";
	dispatch(andExpression->getLHS());
	*mOutput << ") & (";
	dispatch(andExpression->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Equal *equal)
{
	*mOutput << "(";
	dispatch(equal->getLHS());
	*mOutput << ") == (";
	dispatch(equal->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Unequal *unequal)
{
	*mOutput << "(";
	dispatch(unequal->getLHS());
	*mOutput << ") != (";
	dispatch(unequal->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::LessThan *lessThan)
{
	*mOutput << "(";
	dispatch(lessThan->getLHS());
	*mOutput << ") < (";
	dispatch(lessThan->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::LessEqual *lessEqual)
{
	*mOutput << "(";
	dispatch(lessEqual->getLHS());
	*mOutput << ") <= (";
	dispatch(lessEqual->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::GreaterThan *greaterThan)
{
	*mOutput << "(";
	dispatch(greaterThan->getLHS());
	*mOutput << ") > (";
	dispatch(greaterThan->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::GreaterEqual *greaterEqual)
{
	*mOutput << "(";
	dispatch(greaterEqual->getLHS());
	*mOutput << ") >= (";
	dispatch(greaterEqual->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Add *add)
{
	*mOutput << "(";
	dispatch(add->getLHS());
	*mOutput << ") + (";
	dispatch(add->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Subtract *subtract)
{
	*mOutput << "(";
	dispatch(subtract->getLHS());
	*mOutput << ") - (";
	dispatch(subtract->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Multiply *multiply)
{
	*mOutput << "(";
	dispatch(multiply->getLHS());
	*mOutput << ") * (";
	dispatch(multiply->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Divide *divide)
{
	*mOutput << "(";
	dispatch(divide->getLHS());
	*mOutput << ") / (";
	dispatch(divide->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::Modulus *modulus)
{
	*mOutput << "(";
	dispatch(modulus->getLHS());
	*mOutput << ") % (";
	dispatch(modulus->getRHS());
	*mOutput << ")";
}

void generator::C::generate(tree::LogicalNot *logicalNot)
{
	*mOutput << "!(";
	dispatch(logicalNot->getExpression());
	*mOutput << ")";
}

void generator::C::generate(tree::Not *notExpression)
{
	*mOutput << "~(";
	dispatch(notExpression->getExpression());
	*mOutput << ")";
}

void generator::C::generate(tree::Minus *minus)
{
	*mOutput << "-(";
	dispatch(minus->getExpression());
	*mOutput << ")";
}

void generator::C::generate(tree::Execute *execute)
{
	dispatch(execute->getExpression());
	*mOutput << ";" << std::endl;
}

void generator::C::generate(tree::Return *returnExpression)
{
	*mOutput << "return ";
	dispatch(returnExpression->getReturn());
	*mOutput << ";" << std::endl;
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
			*mOutput << "bool *" << identity->getName();
		}
		else
		{
			*mOutput << "bool " << identity->getName();
		}
	}
	else if((integer = dynamic_cast<tree::Int *>(type)))
	{
		if(isReference)
		{
			*mOutput << "int *" << identity->getName();
		}
		else
		{
			*mOutput << "int " << identity->getName();
		}
	}
	else if((floatingPoint = dynamic_cast<tree::Float *>(type)))
	{
		if(isReference)
		{
			*mOutput << "float *" << identity->getName();
		}
		else
		{
			*mOutput << "float " << identity->getName();
		}
	}
	else if((string = dynamic_cast<tree::String *>(type)))
	{
		if(isReference)
		{
			*mOutput << "char **" << identity->getName();
		}
		else
		{
			*mOutput << "char " << identity->getName() << "[" << string->getMaxSize() << "]";
		}
	}
	else if((udt = dynamic_cast<tree::UDT *>(type)))
	{
		ERROR("FIXME");
		//*mOutput << "char*";
	}
	else
	{
		ERROR("Unknown type");
	}
}

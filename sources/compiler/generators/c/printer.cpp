#include "common.h"

#include "compiler/generators/c/printer.h"
#include "compiler/generators/c/mangled.h"

void generator::c::Printer::dispatch(tree::Node *node)
{
	GENERATE_DISPATCH(node, output)
}

void generator::c::Printer::output(tree::Scope *scope)
{
	increaseDepth();

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); dispatch(*i++));
	}

	decreaseDepth();
}

void generator::c::Printer::output(tree::Function *function)
{
	tree::FunctionPrototype *prototype = function->getPrototype();
	tree::Expressions *arguments = prototype->getArguments();

	output(prototype);
	*mOutput << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	for(tree::Scope::NamedNodes::iterator i = function->getNamedNodes().begin(), end = function->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity && !dynamic_cast<tree::Constant *>(identity))
		{
			bool argumentExists = false;

			if(arguments)
			{
				argumentExists = std::find(arguments->begin(), arguments->end(), identity) != arguments->end(); // FIXME, inefficient
			}

			if(!argumentExists)
			{
#ifdef DEBUG
				tree::TypedIdentity *typedIdentity = dynamic_cast<tree::TypedIdentity *>(identity);
				ASSERT(typedIdentity);
#else
				tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(identity);
#endif

				outputTabs();
				outputDeclaration(typedIdentity);
				outputEOS();
			}
		}
	}

	decreaseDepth();

	output(static_cast<tree::Scope *>(function));

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::c::Printer::output(tree::AnonymousScope *anonymousScope)
{
	increaseDepth();

	for(tree::Scope::NamedNodes::iterator i = anonymousScope->getNamedNodes().begin(), end = anonymousScope->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity && !dynamic_cast<tree::Constant *>(identity))
		{
#ifdef DEBUG
			tree::TypedIdentity *typedIdentity = dynamic_cast<tree::TypedIdentity *>(identity);
			ASSERT(typedIdentity);
#else
			tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(identity);
#endif

			outputTabs();
			outputDeclaration(typedIdentity);
			outputEOS();
		}
	}

	decreaseDepth();

	output(static_cast<tree::Scope *>(anonymousScope));
}

void generator::c::Printer::output(tree::Import *import)
{
	tree::FunctionPrototype *functionPrototype = import->getPrototype();

	outputTabs();
	*mOutput << "extern ";
	outputDeclaration(functionPrototype, true);
	*mOutput << "(";

	tree::Expressions *arguments = functionPrototype->getArguments();

	if(arguments)
	{
		tree::Expressions::iterator i = arguments->begin();
		tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(*i++);

		outputDeclaration(typedIdentity, true);

		for(tree::Expressions::iterator end = arguments->end(); i != end; ++i)
		{
			typedIdentity = static_cast<tree::TypedIdentity *>(*i);
			*mOutput << ", ";
			outputDeclaration(typedIdentity, true);
		}
	}
	else
	{
		*mOutput << "void";
	}

	*mOutput << ")";
}

void generator::c::Printer::output(tree::FunctionPrototype *functionPrototype)
{
	outputTabs();
	outputDeclaration(functionPrototype, true);
	*mOutput << "(struct " << mStructName << " *moon$$scope";

	tree::Method *method = tree::node_cast<tree::Method *>(functionPrototype->getTarget());

	if(method)
	{
		tree::Type *type = method->getType();

		*mOutput << ", ";
		outputType(type);
		*mOutput << " moon$$this";
		outputDimensions(type);
	}

	tree::Expressions *arguments = functionPrototype->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(*i);

			*mOutput << ", ";
			outputDeclaration(typedIdentity, true);
		}
	}

	*mOutput << ")";
}

void generator::c::Printer::output(tree::Identity *identity)
{
	ASSERT(identity->getMetadata());
	generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(identity->getMetadata());

	*mOutput << cName->useName;
}

void generator::c::Printer::output(tree::Cast *cast)
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
#ifdef DEBUG
		type->printType();
#endif
		ERROR("Unknown type");
	}

	*mOutput << "(";
	dispatch(cast->getExpression());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::IfExpression *ifExpression)
{
	dispatch(ifExpression->getTest());
	*mOutput << " ? ";
	dispatch(ifExpression->getTrueResult());
	*mOutput << " : ";
	dispatch(ifExpression->getFalseResult());
}

void generator::c::Printer::output(tree::DirectAccess *directAccess)
{
	tree::FunctionCall *functionCall = dynamic_cast<tree::FunctionCall *>(directAccess->getTarget());

	if(functionCall)
	{
		tree::FunctionPrototype *prototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

		ASSERT(prototype->getMetadata());
		generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(prototype->getMetadata());

		ASSERT(!cName->isImport);

		*mOutput << cName->useName << "(";

		tree::Expressions *arguments = functionCall->getArguments();

		*mOutput << "moon$$scope, ";

		dispatch(directAccess->getContainer());

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
	else
	{
		dispatch(directAccess->getContainer());
		*mOutput << ".";
		dispatch(directAccess->getTarget());
	}
}

void generator::c::Printer::output(tree::MessageAccess *messageAccess)
{
	ERROR("FIXME");
}

void generator::c::Printer::output(tree::ArrayAccess *arrayAccess)
{
	dispatch(arrayAccess->getContainer());
	*mOutput << "[";
	dispatch(arrayAccess->getTarget());
	*mOutput << "]";
}

void generator::c::Printer::output(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *prototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

	ASSERT(prototype->getMetadata());
	generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(prototype->getMetadata());

	*mOutput << cName->useName << "(";

	tree::Expressions *arguments = functionCall->getArguments();

	if(cName->isImport)
	{
		if(arguments)
		{
			tree::Expressions::iterator i = arguments->begin();

			dispatch(*i++);

			for(tree::Expressions::iterator end = arguments->end(); i != end; ++i)
			{
				*mOutput << ", ";
				dispatch(*i);
			}
		}
	}
	else
	{
		*mOutput << "moon$$scope";

		if(arguments)
		{
			for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
			{
				*mOutput << ", ";
				dispatch(*i);
			}
		}
	}

	*mOutput << ")";
}

void generator::c::Printer::output(tree::BoolLiteral *boolLiteral)
{
	*mOutput << (boolLiteral->getValue() ? "true" : "false");
}

void generator::c::Printer::output(tree::IntLiteral *intLiteral)
{
	*mOutput << intLiteral->getValue();
}

void generator::c::Printer::output(tree::FloatLiteral *floatLiteral)
{
	*mOutput << floatLiteral->getValue();
}

void generator::c::Printer::output(tree::StringLiteral *stringLiteral)
{
	*mOutput << "\"";

	for(char *i = const_cast<char *>(stringLiteral->getValue().c_str()); *i; i++)
	{
		switch(*i)
		{
			case '\\':
				*mOutput << "\\\\";
				break;

			case '\'':
				*mOutput << "\\\'";
				break;

			case '\"':
				*mOutput << "\\\"";
				break;

			case '\a':
				*mOutput << "\\a";
				break;

			case '\b':
				*mOutput << "\\b";
				break;

			case '\f':
				*mOutput << "\\f";
				break;

			case '\n':
				*mOutput << "\\n";
				break;

			case '\r':
				*mOutput << "\\r";
				break;

			case '\t':
				*mOutput << "\\t";
				break;

			default:
				*mOutput << *i;
				break;
		}
	}

	*mOutput << "\"";
}

void generator::c::Printer::output(tree::Assign *assign)
{
	dispatch(assign->getLHS());
	*mOutput << " = ";
	dispatch(assign->getRHS());
}

void generator::c::Printer::output(tree::LogicalOr *logicalOr)
{
	*mOutput << "(";
	dispatch(logicalOr->getLHS());
	*mOutput << ") || (";
	dispatch(logicalOr->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::LogicalAnd *logicalAnd)
{
	*mOutput << "(";
	dispatch(logicalAnd->getLHS());
	*mOutput << ") && (";
	dispatch(logicalAnd->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Or *orExpression)
{
	*mOutput << "(";
	dispatch(orExpression->getLHS());
	*mOutput << ") | (";
	dispatch(orExpression->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Xor *xorExpression)
{
	*mOutput << "(";
	dispatch(xorExpression->getLHS());
	*mOutput << ") ^ (";
	dispatch(xorExpression->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::And *andExpression)
{
	*mOutput << "(";
	dispatch(andExpression->getLHS());
	*mOutput << ") & (";
	dispatch(andExpression->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Equal *equal)
{
	*mOutput << "(";
	dispatch(equal->getLHS());
	*mOutput << ") == (";
	dispatch(equal->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Unequal *unequal)
{
	*mOutput << "(";
	dispatch(unequal->getLHS());
	*mOutput << ") != (";
	dispatch(unequal->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::LessThan *lessThan)
{
	*mOutput << "(";
	dispatch(lessThan->getLHS());
	*mOutput << ") < (";
	dispatch(lessThan->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::LessEqual *lessEqual)
{
	*mOutput << "(";
	dispatch(lessEqual->getLHS());
	*mOutput << ") <= (";
	dispatch(lessEqual->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::GreaterThan *greaterThan)
{
	*mOutput << "(";
	dispatch(greaterThan->getLHS());
	*mOutput << ") > (";
	dispatch(greaterThan->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::GreaterEqual *greaterEqual)
{
	*mOutput << "(";
	dispatch(greaterEqual->getLHS());
	*mOutput << ") >= (";
	dispatch(greaterEqual->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Add *add)
{
	*mOutput << "(";
	dispatch(add->getLHS());
	*mOutput << ") + (";
	dispatch(add->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Subtract *subtract)
{
	*mOutput << "(";
	dispatch(subtract->getLHS());
	*mOutput << ") - (";
	dispatch(subtract->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Multiply *multiply)
{
	*mOutput << "(";
	dispatch(multiply->getLHS());
	*mOutput << ") * (";
	dispatch(multiply->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Divide *divide)
{
	*mOutput << "(";
	dispatch(divide->getLHS());
	*mOutput << ") / (";
	dispatch(divide->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Modulus *modulus)
{
	*mOutput << "(";
	dispatch(modulus->getLHS());
	*mOutput << ") % (";
	dispatch(modulus->getRHS());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::LogicalNot *logicalNot)
{
	*mOutput << "!(";
	dispatch(logicalNot->getExpression());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Not *notExpression)
{
	*mOutput << "~(";
	dispatch(notExpression->getExpression());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::Minus *minus)
{
	*mOutput << "-(";
	dispatch(minus->getExpression());
	*mOutput << ")";
}

void generator::c::Printer::output(tree::UDT *udt)
{
	ASSERT(udt->getMetadata());
	generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(udt->getMetadata());

	outputTabs();
	*mOutput << "struct " << cName->declarationName << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	tree::Members *members = udt->getMembers();
	ASSERT(members);

	for(tree::Members::iterator i = members->begin(), e = members->end(); i != e; ++i)
	{
		outputTabs();
		outputDeclaration(*i);
		outputEOS();
	}

	decreaseDepth();

	outputTabs();
	*mOutput << "}";
	outputEOS();
}

void generator::c::Printer::output(tree::This *th1s)
{
	*mOutput << "moon$$this";
}

void generator::c::Printer::output(tree::Break *breakStatement)
{
	outputTabs();

	*mOutput << "break";

	outputEOS();
}

void generator::c::Printer::output(tree::Execute *execute)
{
	outputTabs();
	dispatch(execute->getExpression());
	outputEOS();
}

void generator::c::Printer::output(tree::If *ifStatement)
{
	outputTabs();
	*mOutput << "if(";
	dispatch(ifStatement->getTest());
	*mOutput << ")" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	tree::Statement *trueStatement = ifStatement->getTrueStatement();

	if(trueStatement)
	{
		dispatch(trueStatement);
	}

	outputTabs();
	*mOutput << "}" << std::endl;

	tree::Statement *falseStatement = ifStatement->getFalseStatement();

	if(falseStatement)
	{
		if(dynamic_cast<tree::If *>(falseStatement))
		{
			outputTabs();
			*mOutput << "else ";

			dispatch(falseStatement);
		}
		else
		{
			outputTabs();
			*mOutput << "else" << std::endl;

			outputTabs();
			*mOutput << "{" << std::endl;

			dispatch(falseStatement);

			outputTabs();
			*mOutput << "}" << std::endl;
		}
	}

	//outputEOS();
}

void generator::c::Printer::output(tree::For *forStatement)
{
	tree::ComputedArray *computedArray = tree::node_cast<tree::ComputedArray *>(forStatement->getIterable());

	if(computedArray)
	{
		outputTabs();
		*mOutput << "for(";
		outputDeclaration(forStatement->getVariable());
		*mOutput << " = ";
		dispatch(computedArray->getFrom());
		*mOutput << "; ";
		dispatch(forStatement->getVariable());
		*mOutput << " <= ";
		dispatch(computedArray->getTo());
		*mOutput << "; ";

		if(computedArray->getStep())
		{
			dispatch(forStatement->getVariable());
			*mOutput << " += ";
			dispatch(computedArray->getStep());
		}
		else
		{
			// FIXME, need to take type into account...
			dispatch(forStatement->getVariable());
			*mOutput << "++";
		}

		*mOutput << ")" << std::endl;

		outputTabs();
		*mOutput << "{" << std::endl;

		for(tree::Scope::NamedNodes::iterator i = forStatement->getNamedNodes().begin(), end = forStatement->getNamedNodes().end(); i != end; ++i)
		{
			tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

			if(identity != forStatement->getVariable())
			{
				if(identity && !dynamic_cast<tree::Constant *>(identity))
				{
#ifdef DEBUG
					tree::TypedIdentity *typedIdentity = dynamic_cast<tree::TypedIdentity *>(identity);
					ASSERT(typedIdentity);
#else
					tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(identity);
#endif

					outputTabs();
					outputDeclaration(typedIdentity);
					outputEOS();
				}
			}
		}

		output(static_cast<tree::Scope *>(forStatement));

		outputTabs();
		*mOutput << "}" << std::endl;
	}
	else
	{
		tree::Array *array = static_cast<tree::Array *>(forStatement->getIterable()->getType());

		outputTabs();
		*mOutput << "for(unsigned int moon$$iterator = 0; moon$$iterator < ";
		*mOutput << array->getSizeInt();
		*mOutput << "; ++moon$$iterator)" << std::endl;

		outputTabs();
		*mOutput << "{" << std::endl;

		for(tree::Scope::NamedNodes::iterator i = forStatement->getNamedNodes().begin(), end = forStatement->getNamedNodes().end(); i != end; ++i)
		{
			tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

			if(identity && !dynamic_cast<tree::Constant *>(identity))
			{
#ifdef DEBUG
				tree::TypedIdentity *typedIdentity = dynamic_cast<tree::TypedIdentity *>(identity);
				ASSERT(typedIdentity);
#else
				tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(identity);
#endif

				outputTabs();
				outputDeclaration(typedIdentity);
				outputEOS();
			}
		}

		outputTabs();
		dispatch(forStatement->getVariable());
		*mOutput << " = ";
		dispatch(forStatement->getIterable());
		*mOutput << "[moon$$iterator]";
		outputEOS();

		output(static_cast<tree::Scope *>(forStatement));

		outputTabs();
		*mOutput << "}" << std::endl;
	}
}

void generator::c::Printer::output(tree::While *whileStatement)
{
	outputTabs();
	*mOutput << "while(";
	dispatch(whileStatement->getTest());
	*mOutput << ")" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	tree::Statement *loopStatement = whileStatement->getLoopStatement();

	if(loopStatement)
	{
		dispatch(loopStatement);
	}

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::c::Printer::output(tree::Return *returnStatement)
{
	outputTabs();

	if(returnStatement->getReturn())
	{
		*mOutput << "return ";
		dispatch(returnStatement->getReturn());
	}
	else
	{
		*mOutput << "return";
	}

	outputEOS();
}

void generator::c::Printer::outputEOS()
{
	*mOutput << ";" << std::endl;
}

void generator::c::Printer::outputExtern(tree::TypedIdentity *typedIdentity)
{
	*mOutput << "extern ";
	outputDeclaration(typedIdentity);
	outputEOS();
}

void generator::c::Printer::outputDeclaration(tree::TypedIdentity *typedIdentity, bool functionPrototype)
{
	tree::Type *type = typedIdentity->getType();

	ASSERT(typedIdentity->getMetadata());
	generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(typedIdentity->getMetadata());

	outputType(type);

	if(dynamic_cast<tree::String *>(type) && functionPrototype)
	{
		*mOutput << " *" << cName->declarationName; // FIXME, this isn't good...
	}
	else
	{
		*mOutput << " " << cName->declarationName;
		outputDimensions(type);
	}
}

void generator::c::Printer::outputType(tree::Type *type)
{
	tree::Int *integer;
	tree::Float *floatingPoint;
	tree::UDT *udt;
	tree::Array *array;

	ASSERT(type);

	if(dynamic_cast<tree::Void *>(type))
	{
		*mOutput << "void";
	}
	else if(dynamic_cast<tree::Bool *>(type))
	{
		*mOutput << "bool";
	}
	else if((integer = dynamic_cast<tree::Int *>(type)))
	{
		switch(integer->getSizeInt())
		{
			case 8:
				*mOutput << "char";
				break;

			case 16:
				*mOutput << "short";
				break;

			case 32:
				*mOutput << "long";
				break;

			case 64:
				*mOutput << "long long";
				break;

			default:
				ERROR("FIXME");
		}
	}
	else if((floatingPoint = dynamic_cast<tree::Float *>(type)))
	{
		switch(floatingPoint->getSizeInt())
		{
			case 32:
				*mOutput << "float";
				break;

			case 64:
				*mOutput << "double";
				break;

			default:
				ERROR("FIXME");
		}
	}
	else if(dynamic_cast<tree::String *>(type))
	{
		*mOutput << "char";
	}
	else if((udt = dynamic_cast<tree::UDT *>(type)))
	{
		ASSERT(udt->getMetadata());
		generator::c::Mangled *cName = static_cast<generator::c::Mangled *>(udt->getMetadata());

		*mOutput << "struct " << cName->declarationName;
	}
	else if((array = dynamic_cast<tree::Array *>(type)))
	{
		outputType(array->getType()); // FIXME, this could actually just be a loop...
	}
	else
	{
#ifdef DEBUG
		type->printNode();
		type->printType();
#endif
		ERROR("Unknown type");
	}
}

void generator::c::Printer::outputDimensions(tree::Type *type)
{
	tree::String *string;
	tree::Array *array;

	if((string = dynamic_cast<tree::String *>(type)))
	{
		*mOutput << "[" << string->getSizeInt() << "]";
	}
	else if((array = dynamic_cast<tree::Array *>(type)))
	{
		*mOutput << "[";

		if(!dynamic_cast<tree::None *>(array->getSize()))
		{
			*mOutput << array->getSizeInt();
		}

		*mOutput << "]";
		outputDimensions(array->getType());
	}
}

void generator::c::Printer::outputVariablesBegin()
{
	outputTabs();
	*mOutput << "struct " << mStructName << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();
}

void generator::c::Printer::outputVariablesEnd()
{
	decreaseDepth();

	outputTabs();
	*mOutput << "}";
	outputEOS();
}

void generator::c::Printer::outputNewBegin()
{
	outputTabs();
	*mOutput << "struct " << mStructName << " *moon$$" << mProgram->getName() << "New()" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	outputTabs();
	*mOutput << "struct " << mStructName << " *moon$$scope = (struct " << mStructName << " *)malloc(sizeof(struct " << mStructName << "));" << std::endl;
}

void generator::c::Printer::outputNewEnd()
{
	outputTabs();
	*mOutput << "return moon$$scope;" << std::endl;

	decreaseDepth();

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::c::Printer::outputBootstrapMain()
{
	outputTabs();
	*mOutput << "int main(int argc, char *argv[])" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	outputTabs();
	*mOutput << "moon$$" << mProgram->getName() << "$$main(moon$$" << mProgram->getName() << "New());" << std::endl;

	outputTabs();
	*mOutput << "return 0;" << std::endl;

	decreaseDepth();

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::c::Printer::outputPragma(std::string pragma)
{
	*mOutput << pragma << std::endl;
}

void generator::c::Printer::outputRaw(std::string data)
{
	*mOutput << data;
}

void generator::c::Printer::outputTabs()
{
	for(unsigned int i = 0; i < mDepth; i++)
	{
		*mOutput << "\t";
	}
}

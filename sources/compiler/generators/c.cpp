#include <ostream>
#include "compiler/generators.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


class Mangled
{
public:
	Mangled(std::string _declarationName) : declarationName(_declarationName), useName(_declarationName) {}
	Mangled(std::string _declarationName, std::string _useName) : declarationName(_declarationName), useName(_useName) {}

	std::string declarationName;
	std::string useName;
};

class MangleNames : public operation::Operation
{
public:
	static void run(tree::Program *program);

	virtual void visit(tree::Scope *scope);
	virtual void visit(tree::Program *program);
	virtual void visit(tree::Aggregate *aggregate);
	virtual void visit(tree::Use *use);
	virtual void visit(tree::Function *function);

private:
	MangleNames() {}
};

void MangleNames::run(tree::Program *program)
{
	MangleNames operation;
	program->accept(&operation);
}

void MangleNames::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void MangleNames::visit(tree::Program *program)
{
	for(tree::Identities::iterator i = program->getIdentities().begin(), end = program->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;
		Mangled *cName = new Mangled("moon_" + program->getName() + "_" + identity->getName());

		identity->setMetadata(cName);
	}

	visit(static_cast<tree::Scope *>(program));
}

void MangleNames::visit(tree::Aggregate *aggregate)
{
	tree::Program *program = static_cast<tree::Program *>(aggregate->getParent());

	for(tree::Identities::iterator i = aggregate->getIdentities().begin(), end = aggregate->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;
		Mangled *cName;

		if(dynamic_cast<tree::Variable *>(identity) || dynamic_cast<tree::Reference *>(identity))
		{
			std::string mangledName = "moon_" + program->getName() + "_" + identity->getName();
			cName = new Mangled(mangledName, "scope->" + mangledName);
		}
		else
		{
			cName = new Mangled("moon_" + program->getName() + "_" + identity->getName());
		}

		identity->setMetadata(cName);
	}

	visit(static_cast<tree::Scope *>(aggregate));
}

void MangleNames::visit(tree::Use *use)
{
	tree::Aggregate *aggregate = static_cast<tree::Aggregate *>(use->getParent());
	tree::Program *program = static_cast<tree::Program *>(aggregate->getParent());

	for(tree::Identities::iterator i = use->getIdentities().begin(), end = use->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;
		Mangled *cName;

		if(dynamic_cast<tree::Variable *>(identity) || dynamic_cast<tree::Reference *>(identity))
		{
			std::string mangledName = "moon_" + program->getName() + "_" + use->getName() + "_" + identity->getName();
			cName = new Mangled(mangledName, "scope->" + mangledName);
		}
		else
		{
			cName = new Mangled("moon_" + program->getName() + "_" + use->getName() + "_" + identity->getName());
		}

		identity->setMetadata(cName);
	}

	visit(static_cast<tree::Scope *>(use));
}

void MangleNames::visit(tree::Function *function)
{
	tree::FunctionPrototype *prototype = function->getPrototype();

	ASSERT(prototype->getMetadata());
	Mangled *cPrototypeName = static_cast<Mangled *>(prototype->getMetadata());

	tree::Expressions *arguments = prototype->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			tree::Identity *identity = static_cast<tree::Identity *>(*i);
			Mangled *cName = new Mangled(cPrototypeName->useName + "_" + identity->getName());

			identity->setMetadata(cName);
		}
	}

	for(tree::Identities::iterator i = function->getIdentities().begin(), end = function->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;
		Mangled *cName = new Mangled(cPrototypeName->useName + "_" + identity->getName());

		identity->setMetadata(cName);
	}

	visit(static_cast<tree::Scope *>(function));
}

class OutputConstants : public operation::Operation
{
public:
	static void run(generator::C::Printer *printer, tree::Program *program);

	virtual void visit(tree::Scope *scope);
	virtual void visit(tree::Assign *assign);

private:
	OutputConstants() {}

	generator::C::Printer *mPrinter;
};

void OutputConstants::run(generator::C::Printer *printer, tree::Program *program)
{
	OutputConstants operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void OutputConstants::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void OutputConstants::visit(tree::Assign *assign)
{
	tree::Constant *constant = dynamic_cast<tree::Constant *>(assign->getLHS());

	if(constant)
	{
		mPrinter->outputTabs();
		mPrinter->outputDeclaration(constant);
		mPrinter->outputRaw(" = ");
		mPrinter->dispatch(assign->getRHS());
		mPrinter->outputEOS();
	}
}

class OutputVariables : public operation::Operation
{
public:
	static void run(generator::C::Printer *printer, tree::Program *program);

	virtual void visit(tree::Program *program);
	virtual void visit(tree::Aggregate *aggregate);
	virtual void visit(tree::Use *use);

private:
	OutputVariables() {}

	void visitScope(tree::Scope *scope);

	generator::C::Printer *mPrinter;
};

void OutputVariables::run(generator::C::Printer *printer, tree::Program *program)
{
	OutputVariables operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void OutputVariables::visit(tree::Program *program)
{
	for(tree::Identities::iterator i = program->getIdentities().begin(), end = program->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;

		if(dynamic_cast<tree::Variable *>(identity) || dynamic_cast<tree::Reference *>(identity))
		{
			mPrinter->outputExtern(static_cast<tree::TypedIdentity *>(identity));
		}
	}

	mPrinter->outputVariablesBegin();
	visitScope(program);
	mPrinter->outputVariablesEnd();
}

void OutputVariables::visit(tree::Aggregate *aggregate)
{
	for(tree::Identities::iterator i = aggregate->getIdentities().begin(), end = aggregate->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;

		if(dynamic_cast<tree::Variable *>(identity) || dynamic_cast<tree::Reference *>(identity))
		{
			mPrinter->outputTabs();
			mPrinter->outputDeclaration(static_cast<tree::TypedIdentity *>(identity));
			mPrinter->outputEOS();
		}
	}

	visitScope(aggregate);
}

void OutputVariables::visit(tree::Use *use)
{
	for(tree::Identities::iterator i = use->getIdentities().begin(), end = use->getIdentities().end(); i != end; ++i)
	{
		tree::Identity *identity = i->second;

		if(dynamic_cast<tree::Variable *>(identity) || dynamic_cast<tree::Reference *>(identity))
		{
			mPrinter->outputTabs();
			mPrinter->outputDeclaration(static_cast<tree::TypedIdentity *>(identity));
			mPrinter->outputEOS();
		}
	}
}

void OutputVariables::visitScope(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

class OutputFunctionPrototypes : public operation::Operation
{
public:
	static void run(generator::C::Printer *printer, tree::Program *program);

	virtual void visit(tree::Scope *scope);
	virtual void visit(tree::Function *function);

private:
	OutputFunctionPrototypes() {}

	generator::C::Printer *mPrinter;
};

void OutputFunctionPrototypes::run(generator::C::Printer *printer, tree::Program *program)
{
	OutputFunctionPrototypes operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void OutputFunctionPrototypes::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void OutputFunctionPrototypes::visit(tree::Function *function)
{
	mPrinter->output(function->getPrototype());
	mPrinter->outputEOS();
}

class OutputFunctions : public operation::Operation
{
public:
	static void run(generator::C::Printer *printer, tree::Program *program);

	virtual void visit(tree::Scope *scope);
	virtual void visit(tree::Function *function);

private:
	OutputFunctions() {}

	generator::C::Printer *mPrinter;
};

void OutputFunctions::run(generator::C::Printer *printer, tree::Program *program)
{
	OutputFunctions operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void OutputFunctions::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this));
	}
}

void OutputFunctions::visit(tree::Function *function)
{
	mPrinter->output(function);
}

class OutputNew : public operation::Operation
{
public:
	static void run(generator::C::Printer *printer, tree::Program *program);

	virtual void visit(tree::Scope *scope);
	virtual void visit(tree::Program *program);
	virtual void visit(tree::Aggregate *aggregate);
	virtual void visit(tree::Use *use);

private:
	OutputNew() {}

	void outputScope(tree::Scope *scope);

	generator::C::Printer *mPrinter;
};

void OutputNew::run(generator::C::Printer *printer, tree::Program *program)
{
	OutputNew operation;
	operation.mPrinter = printer;
	program->accept(&operation);
}

void OutputNew::visit(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); ++i)
		{
			if(!dynamic_cast<tree::Function *>(*i))
			{
				(*i)->accept(this);
			}
		}
	}
}

void OutputNew::visit(tree::Program *program)
{
	mPrinter->outputNewBegin();
	visit(static_cast<tree::Scope *>(program));
	mPrinter->outputNewEnd();
}

void OutputNew::visit(tree::Aggregate *aggregate)
{
	outputScope(aggregate);
	visit(static_cast<tree::Scope *>(aggregate));
}

void OutputNew::visit(tree::Use *use)
{
	outputScope(use);
}

void OutputNew::outputScope(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); ++i)
		{
			tree::Execute *execute;
			tree::Assign *assign;

			// Only non-scope and non-constant-assigning expressions should be output here
			if(!dynamic_cast<tree::Scope *>(*i)
				&& !((execute = dynamic_cast<tree::Execute *>(*i)) && (assign = dynamic_cast<tree::Assign *>(execute->getExpression())) && dynamic_cast<tree::Constant *>(assign->getLHS()))
				)
			{
				mPrinter->dispatch(*i);
			}
		}
	}
}

void generator::C::run(std::ostream &output, tree::Program *program)
{
	mPrinter.init(output, program);
	generate(program);
}

void generator::C::generate(tree::Program *program)
{
	mangleNames(program);

	mPrinter.outputTabs();
	mPrinter.outputPragma("#include <stdlib.h>");

	outputConstants(program);
	outputVariables(program);
	outputFunctions(program);
	outputNew(program);
}

void generator::C::mangleNames(tree::Program *program)
{
	MangleNames::run(program);
}

void generator::C::outputConstants(tree::Program *program)
{
	OutputConstants::run(&mPrinter, program);
}

void generator::C::outputVariables(tree::Program *program)
{
	OutputVariables::run(&mPrinter, program);
}

void generator::C::outputFunctions(tree::Program *program)
{
	OutputFunctionPrototypes::run(&mPrinter, program);
	OutputFunctions::run(&mPrinter, program);
}

void generator::C::outputNew(tree::Program *program)
{
	OutputNew::run(&mPrinter, program);
}

void generator::C::Printer::dispatch(tree::Node *node)
{
	GENERATE_DISPATCH(node, output)
}

void generator::C::Printer::output(tree::Scope *scope)
{
	increaseDepth();

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); dispatch(*i++));
	}

	decreaseDepth();
}

void generator::C::Printer::output(tree::Function *function)
{
	output(function->getPrototype());
	*mOutput << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	for(tree::Identities::iterator i = function->getIdentities().begin(), end = function->getIdentities().end(); i != end; ++i)
	{
#ifdef DEBUG
		tree::TypedIdentity *typedIdentity = dynamic_cast<tree::TypedIdentity *>(i->second);
		ASSERT(typedIdentity);
#else
		tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(i->second);
#endif

		outputTabs();
		outputDeclaration(typedIdentity);
		outputEOS();
	}

	decreaseDepth();

	output(static_cast<tree::Scope *>(function));

	tree::Statements *statements = function->getStatements();

	// Check if we need a return statement
	if(statements && dynamic_cast<tree::Return *>(*(--statements->end())) == NULL)
	{
		increaseDepth();
		outputTabs();
		*mOutput << "return 0;" << std::endl;
		decreaseDepth();
	}

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::C::Printer::output(tree::FunctionPrototype *functionPrototype)
{
	outputTabs();
	outputDeclaration(functionPrototype);
	*mOutput << "(struct " << mStructName << " *scope";

	tree::Expressions *arguments = functionPrototype->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			tree::TypedIdentity *typedIdentity = static_cast<tree::TypedIdentity *>(*i);

			*mOutput << ", ";
			outputDeclaration(typedIdentity);
		}
	}

	*mOutput << ")";
}

void generator::C::Printer::output(tree::Identity *identity)
{
	ASSERT(identity->getMetadata());
	Mangled *cName = static_cast<Mangled *>(identity->getMetadata());

	*mOutput << cName->useName;
}

void generator::C::Printer::output(tree::Reference *reference)
{
	ASSERT(reference->getMetadata());
	Mangled *cName = static_cast<Mangled *>(reference->getMetadata());

	*mOutput << cName->useName;
}

void generator::C::Printer::output(tree::Cast *cast)
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

void generator::C::Printer::output(tree::DirectAccess *directAccess)
{
	ERROR("FIXME");
}

void generator::C::Printer::output(tree::MessageAccess *messageAccess)
{
	ERROR("FIXME");
}

void generator::C::Printer::output(tree::ArrayAccess *arrayAccess)
{
	ERROR("FIXME");
}

void generator::C::Printer::output(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *prototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

	ASSERT(prototype->getMetadata());
	Mangled *cName = static_cast<Mangled *>(prototype->getMetadata());

	*mOutput << cName->useName << "(scope";

	tree::Expressions *arguments = functionCall->getArguments();

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

void generator::C::Printer::output(tree::NullReference *nullReference)
{
	*mOutput << "NULL";
}

void generator::C::Printer::output(tree::BoolLiteral *boolLiteral)
{
	*mOutput << (boolLiteral->getValue() ? "true" : "false");
}

void generator::C::Printer::output(tree::IntLiteral *intLiteral)
{
	*mOutput << intLiteral->getValue();
}

void generator::C::Printer::output(tree::FloatLiteral *floatLiteral)
{
	*mOutput << floatLiteral->getValue();
}

void generator::C::Printer::output(tree::StringLiteral *stringLiteral)
{
	*mOutput << "\"" << stringLiteral->getValue() << "\"";
}

void generator::C::Printer::output(tree::Assign *assign)
{
	dispatch(assign->getLHS());
	*mOutput << " = ";
	dispatch(assign->getRHS());
}

void generator::C::Printer::output(tree::LogicalOr *logicalOr)
{
	*mOutput << "(";
	dispatch(logicalOr->getLHS());
	*mOutput << ") || (";
	dispatch(logicalOr->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::LogicalAnd *logicalAnd)
{
	*mOutput << "(";
	dispatch(logicalAnd->getLHS());
	*mOutput << ") && (";
	dispatch(logicalAnd->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Or *orExpression)
{
	*mOutput << "(";
	dispatch(orExpression->getLHS());
	*mOutput << ") | (";
	dispatch(orExpression->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Xor *xorExpression)
{
	*mOutput << "(";
	dispatch(xorExpression->getLHS());
	*mOutput << ") ^ (";
	dispatch(xorExpression->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::And *andExpression)
{
	*mOutput << "(";
	dispatch(andExpression->getLHS());
	*mOutput << ") & (";
	dispatch(andExpression->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Equal *equal)
{
	*mOutput << "(";
	dispatch(equal->getLHS());
	*mOutput << ") == (";
	dispatch(equal->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Unequal *unequal)
{
	*mOutput << "(";
	dispatch(unequal->getLHS());
	*mOutput << ") != (";
	dispatch(unequal->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::LessThan *lessThan)
{
	*mOutput << "(";
	dispatch(lessThan->getLHS());
	*mOutput << ") < (";
	dispatch(lessThan->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::LessEqual *lessEqual)
{
	*mOutput << "(";
	dispatch(lessEqual->getLHS());
	*mOutput << ") <= (";
	dispatch(lessEqual->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::GreaterThan *greaterThan)
{
	*mOutput << "(";
	dispatch(greaterThan->getLHS());
	*mOutput << ") > (";
	dispatch(greaterThan->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::GreaterEqual *greaterEqual)
{
	*mOutput << "(";
	dispatch(greaterEqual->getLHS());
	*mOutput << ") >= (";
	dispatch(greaterEqual->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Add *add)
{
	*mOutput << "(";
	dispatch(add->getLHS());
	*mOutput << ") + (";
	dispatch(add->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Subtract *subtract)
{
	*mOutput << "(";
	dispatch(subtract->getLHS());
	*mOutput << ") - (";
	dispatch(subtract->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Multiply *multiply)
{
	*mOutput << "(";
	dispatch(multiply->getLHS());
	*mOutput << ") * (";
	dispatch(multiply->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Divide *divide)
{
	*mOutput << "(";
	dispatch(divide->getLHS());
	*mOutput << ") / (";
	dispatch(divide->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Modulus *modulus)
{
	*mOutput << "(";
	dispatch(modulus->getLHS());
	*mOutput << ") % (";
	dispatch(modulus->getRHS());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::LogicalNot *logicalNot)
{
	*mOutput << "!(";
	dispatch(logicalNot->getExpression());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Not *notExpression)
{
	*mOutput << "~(";
	dispatch(notExpression->getExpression());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Minus *minus)
{
	*mOutput << "-(";
	dispatch(minus->getExpression());
	*mOutput << ")";
}

void generator::C::Printer::output(tree::Execute *execute)
{
	outputTabs();
	dispatch(execute->getExpression());
	outputEOS();
}

void generator::C::Printer::output(tree::Return *returnExpression)
{
	outputTabs();
	*mOutput << "return ";
	dispatch(returnExpression->getReturn());
	outputEOS();
}

void generator::C::Printer::outputEOS()
{
	*mOutput << ";" << std::endl;
}

void generator::C::Printer::outputExtern(tree::TypedIdentity *typedIdentity)
{
	*mOutput << "extern ";
	outputDeclaration(typedIdentity);
	outputEOS();
}

void generator::C::Printer::outputDeclaration(tree::TypedIdentity *typedIdentity)
{
	bool isReference = dynamic_cast<tree::Reference *>(typedIdentity) != NULL;
	tree::Type *type = typedIdentity->getType();
	tree::Bool *boolean;
	tree::Int *integer;
	tree::Float *floatingPoint;
	tree::String *string;
	tree::UDT *udt;

	ASSERT(typedIdentity->getMetadata());
	Mangled *cName = static_cast<Mangled *>(typedIdentity->getMetadata());

	if(dynamic_cast<tree::Constant *>(typedIdentity))
	{
		*mOutput << "const ";
	}

	if((boolean = dynamic_cast<tree::Bool *>(type)))
	{
		if(isReference)
		{
			*mOutput << "bool *" << cName->declarationName;
		}
		else
		{
			*mOutput << "bool " << cName->declarationName;
		}
	}
	else if((integer = dynamic_cast<tree::Int *>(type)))
	{
		if(isReference)
		{
			*mOutput << "int *" << cName->declarationName;
		}
		else
		{
			*mOutput << "int " << cName->declarationName;
		}
	}
	else if((floatingPoint = dynamic_cast<tree::Float *>(type)))
	{
		if(isReference)
		{
			*mOutput << "float *" << cName->declarationName;
		}
		else
		{
			*mOutput << "float " << cName->declarationName;
		}
	}
	else if((string = dynamic_cast<tree::String *>(type)))
	{
		if(isReference)
		{
			*mOutput << "char **" << cName->declarationName;
		}
		else
		{
			*mOutput << "char " << cName->declarationName << "[" << string->getMaxSize() << "]";
		}
	}
	else if((udt = dynamic_cast<tree::UDT *>(type)))
	{
		ERROR("FIXME");
	}
	else
	{
		ERROR("Unknown type");
	}
}

void generator::C::Printer::outputVariablesBegin()
{
	outputTabs();
	*mOutput << "struct " << mStructName << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();
}

void generator::C::Printer::outputVariablesEnd()
{
	decreaseDepth();

	outputTabs();
	*mOutput << "}";
	outputEOS();
}

void generator::C::Printer::outputNewBegin()
{
	outputTabs();
	*mOutput << "struct " << mStructName << " *moon_" << mProgram->getName() << "New()" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	outputTabs();
	*mOutput << "struct " << mStructName << " *scope = (struct " << mStructName << " *)malloc(sizeof(struct " << mStructName << "));" << std::endl;
}

void generator::C::Printer::outputNewEnd()
{
	outputTabs();
	*mOutput << "return scope;" << std::endl;

	decreaseDepth();

	outputTabs();
	*mOutput << "}" << std::endl;
}

void generator::C::Printer::outputPragma(std::string pragma)
{
	*mOutput << pragma << std::endl;
}

void generator::C::Printer::outputRaw(std::string data)
{
	*mOutput << data;
}

void generator::C::Printer::outputTabs()
{
	for(unsigned int i = 0; i < mDepth; i++)
	{
		*mOutput << "\t";
	}
}

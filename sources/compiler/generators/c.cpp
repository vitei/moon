#include <ostream>
#include "compiler/generators.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


class Mangled
{
public:
	Mangled(std::string _declarationName, bool _isImport = false) : declarationName(_declarationName), useName(_declarationName), isImport(_isImport) {}
	Mangled(std::string _declarationName, std::string _useName, bool _isImport = false) : declarationName(_declarationName), useName(_useName), isImport(_isImport) {}

	std::string declarationName;
	std::string useName;
	bool isImport;
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
	virtual void visit(tree::AnonymousScope *anonymousScope);
	virtual void visit(tree::Import *import);

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
		for(tree::Statements::iterator i = statements->begin(); i != statements->end(); (*i++)->accept(this))
			;
	}
}

void MangleNames::visit(tree::Program *program)
{
	for(tree::Scope::NamedNodes::iterator i = program->getNamedNodes().begin(), end = program->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity)
		{
			Mangled *cName = new Mangled("moon$$" + program->getName() + "_" + identity->getName());

			ASSERT(!identity->getMetadata());
			identity->setMetadata(cName);
		}
	}

	visit(static_cast<tree::Scope *>(program));
}

void MangleNames::visit(tree::Aggregate *aggregate)
{
	tree::Program *program = static_cast<tree::Program *>(aggregate->getParent());

	for(tree::Scope::NamedNodes::iterator i = aggregate->getNamedNodes().begin(), end = aggregate->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity)
		{
			Mangled *cName;

			if(dynamic_cast<tree::Variable *>(identity))
			{
				std::string mangledName = "moon$$" + program->getName() + "_" + identity->getName();
				cName = new Mangled(mangledName, "scope->" + mangledName);
			}
			else if(!dynamic_cast<tree::Function *>(identity))
			{
				cName = new Mangled("moon$$" + program->getName() + "_" + identity->getName());
			}
			else
			{
				ERROR("Unknown identity type");
			}

			ASSERT(!identity->getMetadata());
			identity->setMetadata(cName);
		}
	}

	visit(static_cast<tree::Scope *>(aggregate));
}

void MangleNames::visit(tree::Use *use)
{
	tree::Aggregate *aggregate = static_cast<tree::Aggregate *>(use->getParent());
	tree::Program *program = static_cast<tree::Program *>(aggregate->getParent());

	for(tree::Scope::NamedNodes::iterator i = use->getNamedNodes().begin(), end = use->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity)
		{
			Mangled *cName;

			if(dynamic_cast<tree::Variable *>(identity))
			{
				std::string mangledName = "moon$$" + program->getName() + "_" + use->getName() + "_" + identity->getName();
				cName = new Mangled(mangledName, "scope->" + mangledName);
			}
			else
			{
				cName = new Mangled("moon$$" + program->getName() + "_" + use->getName() + "_" + identity->getName());
			}

			ASSERT(!identity->getMetadata());
			identity->setMetadata(cName);
		}
	}

	visit(static_cast<tree::Scope *>(use));
}

void MangleNames::visit(tree::Function *function)
{
	tree::FunctionPrototype *prototype = function->getPrototype();

	ASSERT(prototype->getMetadata());
	Mangled *cPrototypeName = static_cast<Mangled *>(prototype->getMetadata());

	tree::Expressions *arguments = prototype->getArguments();

	for(tree::Scope::NamedNodes::iterator i = function->getNamedNodes().begin(), end = function->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity)
		{
			Mangled *cName = new Mangled(cPrototypeName->useName + "_" + identity->getName());

			ASSERT(!identity->getMetadata());
			identity->setMetadata(cName);
		}
	}

	visit(static_cast<tree::Scope *>(function));
}

void MangleNames::visit(tree::AnonymousScope *anonymousScope)
{
	tree::Function *function = NULL;

	for(tree::Scope *nextScope = anonymousScope->getParent(); nextScope && !function; function = dynamic_cast<tree::Function *>(nextScope), nextScope = nextScope->getParent())
		;

	ASSERT(function);

	tree::FunctionPrototype *prototype = function->getPrototype();

	ASSERT(prototype);
	ASSERT(prototype->getMetadata());
	Mangled *cPrototypeName = static_cast<Mangled *>(prototype->getMetadata());

	for(tree::Scope::NamedNodes::iterator i = anonymousScope->getNamedNodes().begin(), end = anonymousScope->getNamedNodes().end(); i != end; ++i)
	{
		tree::Identity *identity = dynamic_cast<tree::Identity *>(i->second);

		if(identity)
		{
			Mangled *cName = new Mangled(cPrototypeName->useName + "_" + identity->getName());

			identity->setMetadata(cName);
		}
	}

	visit(static_cast<tree::Scope *>(anonymousScope));
}

void MangleNames::visit(tree::Import *import)
{
	tree::FunctionPrototype *prototype = import->getPrototype();

	ASSERT(prototype->getMetadata());

	// FIXME, this is rather ugly right now...
	Mangled *cPrototypeName = static_cast<Mangled *>(prototype->getMetadata());
	delete cPrototypeName;
	cPrototypeName = new Mangled(prototype->getName(), true);
	prototype->setMetadata(cPrototypeName);

	tree::Expressions *arguments = prototype->getArguments();

	if(arguments)
	{
		for(tree::Expressions::iterator i = arguments->begin(), end = arguments->end(); i != end; ++i)
		{
			tree::Identity *identity = static_cast<tree::Identity *>(*i);
			Mangled *cName = new Mangled(identity->getName());

			identity->setMetadata(cName);
		}
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

	std::vector<tree::TypedIdentity *> mIdentities;
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
	for(tree::Scope::NamedNodes::iterator i = program->getNamedNodes().begin(), end = program->getNamedNodes().end(); i != end; ++i)
	{
		if(dynamic_cast<tree::Variable *>(i->second))
		{
			mPrinter->outputExtern(static_cast<tree::TypedIdentity *>(i->second));
		}
	}

	visitScope(program);

	mPrinter->outputVariablesBegin();

	for(std::vector<tree::TypedIdentity *>::iterator i = mIdentities.begin(), e = mIdentities.end(); i != e; ++i)
	{
		mPrinter->outputTabs();
		mPrinter->outputDeclaration(*i);
		mPrinter->outputEOS();
	}

	mPrinter->outputVariablesEnd();
}

void OutputVariables::visit(tree::Aggregate *aggregate)
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

void OutputVariables::visit(tree::Use *use)
{
	for(tree::Scope::NamedNodes::iterator i = use->getNamedNodes().begin(), end = use->getNamedNodes().end(); i != end; ++i)
	{
		if(dynamic_cast<tree::Variable *>(i->second))
		{
			mIdentities.push_back(static_cast<tree::TypedIdentity *>(i->second));
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
	virtual void visit(tree::Import *import);

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

void OutputFunctionPrototypes::visit(tree::Import *import)
{
	//LOG("C FUNCTION");

	mPrinter->output(import);
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
			// Only assign expressions should be output here
			tree::Execute *execute = dynamic_cast<tree::Execute *>(*i);

			if(execute && dynamic_cast<tree::Assign *>(execute->getExpression()))
			{
				mPrinter->dispatch(execute);
			}
		}
	}
}

void generator::C::run(std::ostream &output, tree::Program *program)
{
	mPrinter.init(output, program);
	generate(program);
}

std::string generator::C::getOptions()
{
	return "B";
}

void generator::C::handleOption(char opt, char *optarg, int optopt)
{
	switch(opt)
	{
		case 'B':
			mIsBoostrapped = true;
			break;
	}
}

std::string generator::C::optionsString()
{
	return "[-B]";
}

std::string generator::C::optionsHelpString()
{
	return "\t-B Output bootstrap code";
}

void generator::C::generate(tree::Program *program)
{
	mangleNames(program);

	mPrinter.outputTabs();
	mPrinter.outputPragma("#include <stdlib.h>");

	mPrinter.outputTabs();
	mPrinter.outputPragma("#include <stdbool.h>");

	outputVariables(program);
	outputFunctions(program);
	outputNew(program);

	if(mIsBoostrapped)
	{
		mPrinter.outputBootstrapMain();
	}
}

void generator::C::mangleNames(tree::Program *program)
{
	MangleNames::run(program);
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

void generator::C::Printer::output(tree::AnonymousScope *anonymousScope)
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

void generator::C::Printer::output(tree::Import *import)
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

	*mOutput << ")";
}

void generator::C::Printer::output(tree::FunctionPrototype *functionPrototype)
{
	outputTabs();
	outputDeclaration(functionPrototype, true);
	*mOutput << "(struct " << mStructName << " *scope";

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

void generator::C::Printer::output(tree::Identity *identity)
{
	ASSERT(identity->getMetadata());
	Mangled *cName = static_cast<Mangled *>(identity->getMetadata());

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
#ifdef DEBUG
		type->printType();
#endif
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
	dispatch(arrayAccess->getContainer());
	*mOutput << "[";
	dispatch(arrayAccess->getTarget());
	*mOutput << "]";
}

void generator::C::Printer::output(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *prototype = static_cast<tree::FunctionPrototype *>(functionCall->getPrototype());

	ASSERT(prototype->getMetadata());
	Mangled *cName = static_cast<Mangled *>(prototype->getMetadata());

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
		*mOutput << "scope";

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

void generator::C::Printer::output(tree::If *ifStatement)
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

void generator::C::Printer::output(tree::While *whileStatement)
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

void generator::C::Printer::output(tree::Return *returnStatement)
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

void generator::C::Printer::outputDeclaration(tree::TypedIdentity *typedIdentity, bool functionPrototype)
{
	tree::Type *type = typedIdentity->getType();

	ASSERT(typedIdentity->getMetadata());
	Mangled *cName = static_cast<Mangled *>(typedIdentity->getMetadata());

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

void generator::C::Printer::outputType(tree::Type *type)
{
	tree::Int *integer;
	tree::Float *floatingPoint;
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
	else if(dynamic_cast<tree::UDT *>(type))
	{
		ERROR("FIXME");
	}
	else if((array = dynamic_cast<tree::Array *>(type)))
	{
		outputType(array->getType()); // FIXME, this could actually just be a loop...
	}
	else
	{
#ifdef DEBUG
		type->printType();
#endif
		ERROR("Unknown type");
	}
}

void generator::C::Printer::outputDimensions(tree::Type *type)
{
	tree::String *string;
	tree::Array *array;

	if((string = dynamic_cast<tree::String *>(type)))
	{
		*mOutput << "[" << string->getSizeInt() << "]";
	}
	else if((array = dynamic_cast<tree::Array *>(type)))
	{
		*mOutput << "[" << array->getSizeInt() << "]";
		outputDimensions(array->getType());
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
	*mOutput << "struct " << mStructName << " *moon$$" << mProgram->getName() << "New()" << std::endl;

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

void generator::C::Printer::outputBootstrapMain()
{
	outputTabs();
	*mOutput << "int main(int argc, char *argv[])" << std::endl;

	outputTabs();
	*mOutput << "{" << std::endl;

	increaseDepth();

	outputTabs();
	*mOutput << "moon$$" << mProgram->getName() << "_main(moon$$" << mProgram->getName() << "New());" << std::endl;

	outputTabs();
	*mOutput << "return 0;" << std::endl;

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

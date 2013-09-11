#include <sstream>
#include "compiler/error.h"
#include "compiler/operations.h"
#include "compiler/tree.h"


bool operation::ResolveIdentities::run(tree::Program *program)
{
	operation::ResolveIdentities operation;

	operation.mValidated = true;

	if(program->getStatements())
	{
		operation.add(NULL, program);
		operation.process();
	}

	ASSERT(operation.mNodeMap.size() == 0);

	return operation.mValidated;
}

void operation::ResolveIdentities::add(tree::Scope *parentScope, tree::Scope *scope)
{
	// We do this so that "shared" functions are placed into their effective scopes when processed.
	mVisitList[parentScope].push(scope);
}

void operation::ResolveIdentities::process()
{
	std::queue<tree::Scope *> processNext;

	processNext.push(NULL);

	while(!processNext.empty())
	{
		tree::Scope *scanScope = processNext.front();
		processNext.pop();

		std::queue<tree::Scope *> &visitNext = mVisitList[scanScope];

		while(!visitNext.empty())
		{
			mCurrentScope = visitNext.front();
			visitNext.pop();

			tree::Function *function = dynamic_cast<tree::Function *>(mCurrentScope);

			if(function)
			{
				processFunctionParameters(function);
			}

			tree::Statements *statements = mCurrentScope->getStatements();

			if(statements)
			{
				for(tree::Statements::iterator i = statements->begin(); i != statements->end();)
				{
					(*i)->accept(this);

					tree::Statement *statement = static_cast<tree::Statement *>(mNodeMap.top());
					mNodeMap.pop();

					if(statement)
					{
						*i = statement;
						++i;
					}
					else
					{
						i = statements->erase(i);
					}
				}
			}

			processNext.push(mCurrentScope);
		}
	}
}

void operation::ResolveIdentities::dispatch(tree::Scope *scope)
{
	LOG("ResolveIdentities::dispatch::Scope");

	operation::Restructure::dispatch(static_cast<tree::Statement *>(scope));

	add(mCurrentScope, scope);
}

void operation::ResolveIdentities::dispatch(tree::Function *function)
{
	LOG("ResolveIdentities::dispatch::Function");

	tree::FunctionPrototype *functionPrototype = NULL;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, function->getPrototype());
	function->setPrototype(functionPrototype);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(function));

	add(function->getOriginalScope() ? function->getOriginalScope() : mCurrentScope, function);
}

void operation::ResolveIdentities::dispatch(tree::Import *import)
{
	tree::FunctionPrototype *functionPrototype = NULL;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, import->getPrototype());
	import->setPrototype(functionPrototype);

	// For this, operation there is no point in processing the parameters for imports...

	operation::Restructure::dispatch(static_cast<tree::Statement *>(import));
}

void operation::ResolveIdentities::dispatch(tree::UDT *udt)
{
	tree::UDT *oldUDT = mCurrentUDT;

	mCurrentUDT = udt;

	tree::Members *members = udt->getMembers();
	ASSERT(members);

	for(tree::Members::iterator i = members->begin(); i != members->end();)
	{
		(*i)->accept(this);

		tree::Member *member = static_cast<tree::Member *>(mNodeMap.top());
		mNodeMap.pop();

		if(member)
		{
			*i = member;
			++i;
		}
		else
		{
			i = members->erase(i);
		}
	}

	mCurrentUDT = oldUDT;

	operation::Restructure::dispatch(static_cast<tree::Type *>(udt));
}

void operation::ResolveIdentities::visit(tree::TypeDefinition *typeDefinition)
{
	LOG("ResolveIdentities::visit::TypeDefinition");

	ASSERT(!mCurrentUDT);

	try
	{
		mCurrentScope->mapNamedNode(typeDefinition->getName(), typeDefinition->getType());
	}
	catch(behaviour::NamedMap::ExistsException &e)
	{
		std::string error = "The identifier \"" + typeDefinition->getName() + "\" is already defined";
		error::enqueue(e.conflict->getLocation(), e.node->getLocation(), error);
	}

	operation::Restructure::visit(typeDefinition);
}

void operation::ResolveIdentities::visit(tree::Identity *identity)
{
	LOG("ResolveIdentities::visit::Identity");

	try
	{
		behaviour::NamedMap *map = mCurrentUDT ? static_cast<behaviour::NamedMap *>(mCurrentUDT) : static_cast<behaviour::NamedMap *>(mCurrentScope);
		map->mapNamedNode(identity->getName(), identity);
	}
	catch(behaviour::NamedMap::ExistsException &e)
	{
		std::string error = "The identifier \"" + identity->getName() + "\" is already defined";
		error::enqueue(e.conflict->getLocation(), e.node->getLocation(), error);
	}

	operation::Restructure::visit(identity);
}

void operation::ResolveIdentities::visit(tree::Expression *expression)
{
	LOG("ResolveIdentities::visit::Expression");

	// FIXME, this is a terrible hack...

	std::stack<tree::Node *> nodeMapClone = mNodeMap;

	for(;;)
	{
		try
		{
			//operation::Restructure::visit(expression);
			operation::Restructure::visit(static_cast<tree::Node *>(expression));
			break;
		}
		catch(tree::FunctionCall::InvalidFunctionException &e)
		{
			tree::Identifier *identifier = (tree::Identifier *)e.functionCall->getPrototype();
			std::string error = "The identifier \"" + identifier->getName() + "\" does not refer to a function";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}
		catch(tree::FunctionCall::InvalidArgumentsException &e)
		{
			tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(e.functionCall->getPrototype());
			std::stringstream error;

			error << "The function \"" << functionPrototype->getName() << "\" accepts " << functionPrototype->getArguments()->size() << " parameters, not " << e.functionCall->getArguments()->size();

			error::enqueue(functionPrototype->getLocation(), e.functionCall->getLocation(), error.str());
			e.reset();
			mNodeMap = nodeMapClone;
		}
		catch(tree::Expression::InvalidException &e)
		{
			ASSERT(e.expression);
			ASSERT(dynamic_cast<tree::Identifier *>(e.expression));

			tree::Identifier *identifier = (tree::Identifier *)e.expression;
			std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}
	}
}

void operation::ResolveIdentities::visit(tree::Assign *assign)
{
	LOG("ResolveIdentities::visit::Assign");

	assign->Expression::childAccept(this);

	if(assign->getRHS())
	{
		assign->getRHS()->accept(this);
	}

	if(assign->getLHS())
	{
		mCanCreateIdentifier = true;
		assign->getLHS()->accept(this);
		mCanCreateIdentifier = false;
	}

	operation::Restructure::visit(assign);
}

tree::Node *operation::ResolveIdentities::restructure(tree::Identifier *identifier)
{
	tree::Node *r = NULL;

	LOG("ResolveIdentities::restructure::Identifier");

	try
	{
		r = getCurrentScope()->findNamedNode(identifier);
	}
	catch(behaviour::NamedMap::NotFoundException &e)
	{
		if(mCanCreateIdentifier)
		{
			tree::Variable *variable = new tree::Variable(NULL, identifier->getName());

			variable->setLocation(identifier->getLocation());
			getCurrentScope()->mapNamedNode(variable->getName(), variable);

			r = variable;
		}
		else
		{
			std::string error = "The identifier \"" + identifier->getName() + "\" does not exist";
			error::enqueue(identifier->getLocation(), error);
		}
	}

	//delete identifier; // Can't do this here as it'll cause problems if an exception is thrown...

	return r;
}

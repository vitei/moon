#include "common.h"

#include <sstream>
#include "compiler/error.h"
#include "compiler/tree.h"
#include "compiler/operations/resolve_identities.h"

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

			mCurrentMap = mCurrentScope;

			tree::Function *function = tree::node_cast<tree::Function *>(mCurrentScope);

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
	mCanMapIdentifier = false;
	operation::Restructure::dispatch(import);
	mCanMapIdentifier = true;
}

void operation::ResolveIdentities::dispatch(tree::UDT *udt)
{
	behaviour::NamedMap *oldMap = mCurrentMap;

	mCurrentMap = udt;

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

	mCurrentMap = oldMap;

	operation::Restructure::dispatch(static_cast<tree::Type *>(udt));
}

void operation::ResolveIdentities::visit(tree::TypeDefinition *typeDefinition)
{
	LOG("ResolveIdentities::visit::TypeDefinition");

	if(mCanMapIdentifier)
	{
		try
		{
			mCurrentMap->mapNamedNode(typeDefinition->getName(), typeDefinition->getType());
		}
		catch(behaviour::NamedMap::ExistsException &e)
		{
			std::string error = "The type \"" + typeDefinition->getName() + "\" is already defined";
			error::enqueue(e.conflict->getLocation(), e.node->getLocation(), error);
		}
	}

	operation::Restructure::visit(typeDefinition);
}

void operation::ResolveIdentities::visit(tree::Identity *identity)
{
	LOG("ResolveIdentities::visit::Identity");

	if(mCanMapIdentifier)
	{
		try
		{
			mCurrentMap->mapNamedNode(identity->getName(), identity);
		}
		catch(behaviour::NamedMap::ExistsException &e)
		{
			std::string error = "The identifier \"" + identity->getName() + "\" is already defined";
			error::enqueue(e.conflict->getLocation(), e.node->getLocation(), error);
		}
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
			ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

			tree::Identifier *identifier = (tree::Identifier *)e.expression;
			std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

			error::enqueue(identifier->getLocation(), error);
			e.reset();
			mNodeMap = nodeMapClone;
		}

		// On error, restore the default resolution scope
		mCurrentMap = mCurrentScope;
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
		// Only create the identifier if we're assigning to an identifier
		mCanCreateIdentifier = tree::node_cast<tree::Identifier *>(assign->getLHS());

		assign->getLHS()->accept(this);
		mCanCreateIdentifier = false;
	}

	operation::Restructure::visit(assign);
}

void operation::ResolveIdentities::visit(tree::ArrayAccess *arrayAccess)
{
	LOG("ResolveIdentities::visit::ArrayAccess");

	arrayAccess->Expression::childAccept(this);

	if(arrayAccess->getContainer())
	{
		arrayAccess->getContainer()->accept(this);
	}

	if(arrayAccess->getTarget())
	{
		behaviour::NamedMap *oldMap = mCurrentMap;

		mCurrentMap = mCurrentScope;
		arrayAccess->getTarget()->accept(this);
		mCurrentMap = oldMap;
	}

	operation::Restructure::visit(arrayAccess);
}

void operation::ResolveIdentities::visit(tree::DirectAccess *directAccess)
{
	LOG("ResolveIdentities::visit::DirectAccess");

	directAccess->Expression::childAccept(this);

	if(directAccess->getContainer())
	{
		directAccess->getContainer()->accept(this);
	}

	if(directAccess->getTarget())
	{
		tree::Expression *container = tree::node_cast<tree::Expression *>(directAccess->getContainer());
		tree::Type *type;

		if(container && (type = container->getType()) && type->isResolved())
		{
			tree::UDT *udt = tree::node_cast<tree::UDT *>(type);;

			// FIXME, is this the best??
			// We could do this using exceptions...
			if(udt)
			{
				behaviour::NamedMap *oldMap = mCurrentMap;

				mCurrentMap = udt;
				directAccess->getTarget()->accept(this);
				mCurrentMap = oldMap;
			}
			else
			{
				std::string error = std::string("No members for type \"") + type->getTypeName() + "\"";
				error::enqueue(directAccess->getLocation(), error);

				mNodeMap.push(NULL); // FIXME
			}
		}
		else
		{
			mNodeMap.push(directAccess->getTarget());
			mValidated = false;
		}
	}

	operation::Restructure::visit(directAccess);
}

void operation::ResolveIdentities::visit(tree::Member *member)
{
	LOG("ResolveIdentities::visit::Member");

	member->Node::childAccept(this);

	if(member->getType())
	{
		behaviour::NamedMap *oldMap = mCurrentMap;

		mCurrentMap = mCurrentScope;
		member->getType()->accept(this);
		mCurrentMap = oldMap;
	}

	visit(static_cast<tree::Identity *>(member));
}

tree::Node *operation::ResolveIdentities::restructure(tree::Identifier *identifier)
{
	tree::Node *r = NULL;

	LOG("ResolveIdentities::restructure::Identifier");

	try
	{
		r = mCurrentMap->findNamedNode(identifier);
	}
	catch(behaviour::NamedMap::NotFoundException &e)
	{
		if(mCanCreateIdentifier)
		{
			tree::Variable *variable = new tree::Variable(NULL, identifier->getName());

			variable->setLocation(identifier->getLocation());
			mCurrentMap->mapNamedNode(variable->getName(), variable);

			r = variable;
		}
		else
		{
			tree::UDT *udt = dynamic_cast<tree::UDT *>(mCurrentMap);

			if(udt)
			{
				std::string error = "The type \"" + udt->getName() + "\" does not contain a member named \"" + identifier->getName() + "\"";
				error::enqueue(identifier->getLocation(), error);
			}
			else
			{
				std::string error = "The identifier \"" + identifier->getName() + "\" does not exist";
				error::enqueue(identifier->getLocation(), error);
			}
		}
	}

	//delete identifier; // Can't do this here as it'll cause problems if an exception is thrown...

	return r;
}

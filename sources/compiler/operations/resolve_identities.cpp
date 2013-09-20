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
	operation::Restructure::dispatch(static_cast<tree::Type *>(udt));

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
}

void operation::ResolveIdentities::dispatch(tree::Expression *expression)
{
	tree::Type *type = NULL;

	RESTRUCTURE_GET(type, tree::Type, expression->getType());

	try
	{
		expression->setType(type);
	}
	catch(tree::Expression::InvalidTypeException &e)
	{
		ASSERT(e.expression);
		ASSERT(e.expression->getType());
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression->getType()));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression->getType());
		std::string error = "The identifier \"" + identifier->getName() + "\" does not refer to a type";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	operation::Restructure::dispatch(static_cast<tree::Node *>(expression));
}

void operation::ResolveIdentities::dispatch(tree::Access *access)
{
	tree::Expression *target = NULL;
	tree::Expression *container = NULL;

	RESTRUCTURE_GET(target, tree::Expression, access->getTarget());
	RESTRUCTURE_GET(container, tree::Expression, access->getContainer());

	try
	{
		access->setContainer(container);
	}
	catch(tree::Access::InvalidContainerException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" is an invalid container";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	try
	{
		access->setTarget(target);
	}
	catch(tree::Access::InvalidTargetException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" is an invalid target";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	dispatch(static_cast<tree::Expression *>(access));
}

void operation::ResolveIdentities::dispatch(tree::BinaryOperation *binaryOperation)
{
	tree::Expression *rhs = NULL;
	tree::Expression *lhs = NULL;

	RESTRUCTURE_GET(rhs, tree::Expression, binaryOperation->getRHS());
	RESTRUCTURE_GET(lhs, tree::Expression, binaryOperation->getLHS());

	try
	{
		binaryOperation->setLHS(lhs);
	}
	catch(tree::BinaryOperation::InvalidLHSException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	try
	{
		binaryOperation->setRHS(rhs);
	}
	catch(tree::BinaryOperation::InvalidRHSException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	operation::Restructure::dispatch(static_cast<tree::Operation *>(binaryOperation));
}

void operation::ResolveIdentities::dispatch(tree::UnaryOperation *unaryOperation)
{
	tree::Expression *expression = NULL;

	RESTRUCTURE_GET(expression, tree::Expression, unaryOperation->getExpression());

	try
	{
		unaryOperation->setExpression(expression);
	}
	catch(tree::UnaryOperation::InvalidException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	operation::Restructure::dispatch(static_cast<tree::Operation *>(unaryOperation));
}

void operation::ResolveIdentities::dispatch(tree::Assign *assign)
{
	tree::Expression *lhs = NULL;
	tree::Expression *rhs = NULL;

	RESTRUCTURE_GET(lhs, tree::Expression, assign->getLHS());
	RESTRUCTURE_GET(rhs, tree::Expression, assign->getRHS());

	try
	{
		assign->setLHS(lhs);
	}
	catch(tree::BinaryOperation::InvalidLHSException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	try
	{
		assign->setRHS(rhs);
	}
	catch(tree::BinaryOperation::InvalidRHSException &e)
	{
		ASSERT(e.expression);
		ASSERT(tree::node_cast<tree::Identifier *>(e.expression));

		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.expression);
		std::string error = "The identifier \"" + identifier->getName() + "\" cannot be used in an expression";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}

	dispatch(static_cast<tree::Expression *>(assign));
}

void operation::ResolveIdentities::dispatch(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *functionPrototype = NULL;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, functionCall->getPrototype());

	try
	{
		functionCall->setPrototype(functionPrototype);
	}
	catch(tree::FunctionCall::InvalidFunctionException &e)
	{
		tree::Identifier *identifier = tree::node_cast<tree::Identifier *>(e.functionCall->getPrototype());
		ASSERT(identifier);

		std::string error = "The identifier \"" + identifier->getName() + "\" does not refer to a function";

		error::enqueue(identifier->getLocation(), error);
		e.reset();
	}
	catch(tree::FunctionCall::InvalidArgumentsException &e)
	{
		tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(e.functionCall->getPrototype());
		std::stringstream error;

		if(functionPrototype->getArguments())
		{
			error << "The function \"" << functionPrototype->getName() << "\" accepts " << functionPrototype->getArguments()->size() << " parameters, " << e.functionCall->getArguments()->size() << " passed";
		}
		else
		{
			error << "The function \"" << functionPrototype->getName() << "\" accepts no parameters, " << e.functionCall->getArguments()->size() << " passed";
		}

		error::enqueue(functionPrototype->getLocation(), e.functionCall->getLocation(), error.str());
		e.reset();
	}

	dispatch(static_cast<tree::Expression *>(functionCall));

	tree::Expressions *expressions = functionCall->getArguments();

	if(expressions)
	{
		for(tree::Expressions::iterator i = expressions->begin(); i != expressions->end();)
		{
			(*i)->accept(this);

			tree::Expression *expression = static_cast<tree::Expression *>(mNodeMap.top());
			mNodeMap.pop();

			if(expression)
			{
				*i = expression;
				++i;
			}
			else
			{
				i = expressions->erase(i);
			}
		}
	}
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

void operation::ResolveIdentities::visit(tree::FunctionPrototype *functionPrototype)
{
	LOG("ResolveIdentities::visit::FunctionPrototype");

	if(mCanMapIdentifier)
	{
		try
		{
			tree::Type *type = dynamic_cast<tree::Type *>(mCurrentMap);

			if(type)
			{
				mCurrentScope->mapAssociatedNamedNode(type, functionPrototype->getName(), functionPrototype);
			}
			else
			{
				mCurrentMap->mapNamedNode(functionPrototype->getName(), functionPrototype);
			}
		}
		catch(behaviour::NamedMap::ExistsException &e)
		{
			std::string error = "The identifier \"" + functionPrototype->getName() + "\" is already defined";
			error::enqueue(e.conflict->getLocation(), e.node->getLocation(), error);
		}
	}

	operation::Restructure::visit(functionPrototype);
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
			behaviour::NamedMap *oldMap = mCurrentMap;

			mCurrentMap = type;
			directAccess->getTarget()->accept(this);
			mCurrentMap = oldMap;
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

void operation::ResolveIdentities::visit(tree::Method *method)
{
	LOG("ResolveIdentities::visit::Method");

	tree::Type *type = tree::node_cast<tree::Type *>(method->getType());

	if(type && type->isResolved())
	{
		method->Scope::childAccept(this);

		if(method->getPrototype())
		{
			behaviour::NamedMap *oldMap = mCurrentMap;

			mCurrentMap = type;
			method->getPrototype()->accept(this);
			mCurrentMap = oldMap;
		}

		if(method->getType())
		{
			method->getType()->accept(this);
		}

		operation::Restructure::visit(static_cast<tree::Function *>(method));
	}
	else
	{
		// Can't process this method properly yet...
		// We can only process the type...
		if(method->getType())
		{
			method->getType()->accept(this);
		}

		RESTRUCTURE_GET(type, tree::Type, method->getType());
		method->setType(type);

		mNodeMap.push(method);

		// Can't ensure all members are verified yet
		mUnmappedMethods = true;

		mValidated = false;
	}
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
			tree::Type *type = dynamic_cast<tree::Type *>(mCurrentMap);

			if(type)
			{
				try
				{
					r = mCurrentScope->findAssociatedNamedNode(type, identifier);
				}
				catch(behaviour::NamedMap::NotFoundException &e)
				{
					if(!mUnmappedMethods)
					{
						std::string error = std::string("The type \"") + type->getTypeName() + "\" does not contain a member named \"" + identifier->getName() + "\"";
						error::enqueue(identifier->getLocation(), error);
					}
				}
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

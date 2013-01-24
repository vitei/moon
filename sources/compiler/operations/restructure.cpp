#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::Restructure::visit(tree::Node *node)
{
	GENERATE_DISPATCH(node, dispatch)
}

void operation::Restructure::dispatch(tree::Node *node)
{
	mNodeMap.push(node->restructure(this));
}

void operation::Restructure::dispatch(tree::Expression *expression)
{
	tree::Type *type = NULL;

	if(expression->getType())
	{
		type = static_cast<tree::Type *>(mNodeMap.top());
		mNodeMap.pop();
	}

	expression->setType(type);

	operation::Restructure::dispatch(static_cast<tree::Node *>(expression));
}

void operation::Restructure::dispatch(tree::Access *access)
{
	tree::Expression *container = NULL;
	tree::Expression *target = NULL;

	if(access->getTarget())
	{
		target = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(access->getContainer())
	{
		container = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	access->setContainer(container);
	access->setTarget(target);

	operation::Restructure::dispatch(static_cast<tree::Expression *>(access));
}

void operation::Restructure::dispatch(tree::UnaryExpression *unaryExpression)
{
	tree::Expression *expression = NULL;

	if(unaryExpression->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	unaryExpression->setExpression(expression);

	operation::Restructure::dispatch(static_cast<tree::Expression *>(unaryExpression));
}

void operation::Restructure::dispatch(tree::BinaryExpression *binaryExpression)
{
	tree::Expression *lhs = NULL;
	tree::Expression *rhs = NULL;

	if(binaryExpression->getRHS())
	{
		rhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(binaryExpression->getLHS())
	{
		lhs = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	binaryExpression->setLHS(lhs);
	binaryExpression->setRHS(rhs);

	operation::Restructure::dispatch(static_cast<tree::Expression *>(binaryExpression));
}

void operation::Restructure::dispatch(tree::Cast *cast)
{
	tree::Expression *expression = NULL;

	if(cast->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	cast->setExpression(expression);

	operation::Restructure::dispatch(static_cast<tree::Expression *>(cast));
}

void operation::Restructure::dispatch(tree::If *ifStatement)
{
	tree::Expression *test = NULL;
	tree::Statement *trueStatement = NULL;
	tree::Statement *falseStatement = NULL;

	if(ifStatement->getFalseStatement())
	{
		falseStatement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(ifStatement->getTrueStatement())
	{
		trueStatement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(ifStatement->getTest())
	{
		test = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	ifStatement->setTest(test);
	ifStatement->setTrueStatement(trueStatement);
	ifStatement->setFalseStatement(falseStatement);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(ifStatement));
}

void operation::Restructure::dispatch(tree::Import *import)
{
	tree::FunctionPrototype *functionPrototype = NULL;

	if(import->getPrototype())
	{
		functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
		mNodeMap.pop();
	}

	import->setPrototype(functionPrototype);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(import));
}

void operation::Restructure::dispatch(tree::FunctionCall *functionCall)
{
	tree::Expression *functionPrototype = NULL;

	if(functionCall->getPrototype())
	{
		functionPrototype = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	functionCall->setFunctionPrototype(functionPrototype);

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

	operation::Restructure::dispatch(static_cast<tree::Expression *>(functionCall));
}

void operation::Restructure::dispatch(tree::Scoping *scoping)
{
	tree::Statement *statement = NULL;

	if(scoping->getScoped())
	{
		statement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();
	}

	scoping->setScoped(statement);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(scoping));
}

void operation::Restructure::dispatch(tree::Execute *execute)
{
	tree::Expression *expression = NULL;

	if(execute->getExpression())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	execute->setExpression(expression);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(execute));
}

void operation::Restructure::dispatch(tree::Return *opReturn)
{
	tree::Expression *expression = NULL;

	if(opReturn->getReturn())
	{
		expression = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	opReturn->setReturn(expression);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(opReturn));
}

void operation::Restructure::dispatch(tree::SetState *setState)
{
	tree::State *state = NULL;

	if(setState->getState())
	{
		state = static_cast<tree::State *>(mNodeMap.top());
		mNodeMap.pop();
	}

	setState->setState(state);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(setState));
}

void operation::Restructure::dispatch(tree::FunctionPrototype *functionPrototype)
{
	tree::Expressions *expressions = functionPrototype->getArguments();

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

	operation::Restructure::dispatch(static_cast<tree::TypedIdentity *>(functionPrototype));
}

void operation::Restructure::dispatch(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = static_cast<tree::FunctionPrototype *>(mNodeMap.top());
	mNodeMap.pop();

	function->setPrototype(functionPrototype);

	tree::Statements *statements = function->getStatements();

	if(statements)
	{
		tree::Scope *currentScope = mCurrentScope;

		mCurrentScope = function;

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

		mCurrentScope = currentScope;
	}

	operation::Restructure::dispatch(static_cast<tree::Statement *>(function));
}

void operation::Restructure::dispatch(tree::Scope *scope)
{
	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		tree::Scope *currentScope = mCurrentScope;

		mCurrentScope = scope;

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

		mCurrentScope = currentScope;
	}

	operation::Restructure::dispatch(static_cast<tree::Statement *>(scope));
}

void operation::Restructure::dispatch(tree::While *whileStatement)
{
	tree::Expression *test = NULL;
	tree::Statement *loopStatement = NULL;

	if(whileStatement->getLoopStatement())
	{
		loopStatement = static_cast<tree::Statement *>(mNodeMap.top());
		mNodeMap.pop();
	}

	if(whileStatement->getTest())
	{
		test = static_cast<tree::Expression *>(mNodeMap.top());
		mNodeMap.pop();
	}

	whileStatement->setTest(test);
	whileStatement->setLoopStatement(loopStatement);

	operation::Restructure::dispatch(static_cast<tree::Statement *>(whileStatement));
}

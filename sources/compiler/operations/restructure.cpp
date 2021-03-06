#include "common.h"

#include "compiler/tree.h"
#include "compiler/operations/restructure.h"

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
	tree::Type *type = nullptr;

	RESTRUCTURE_GET(type, tree::Type, expression->getType());
	expression->setType(type);

	dispatch(static_cast<tree::Node *>(expression));
}

void operation::Restructure::dispatch(tree::Access *access)
{
	tree::Expression *target = nullptr;
	tree::Expression *container = nullptr;

	RESTRUCTURE_GET(target, tree::Expression, access->getTarget());
	access->setTarget(target);

	RESTRUCTURE_GET(container, tree::Expression, access->getContainer());
	access->setContainer(container);

	dispatch(static_cast<tree::Expression *>(access));
}

void operation::Restructure::dispatch(tree::BinaryOperation *binaryOperation)
{
	tree::Expression *rhs = nullptr;
	tree::Expression *lhs = nullptr;

	RESTRUCTURE_GET(rhs, tree::Expression, binaryOperation->getRHS());
	binaryOperation->setRHS(rhs);

	RESTRUCTURE_GET(lhs, tree::Expression, binaryOperation->getLHS());
	binaryOperation->setLHS(lhs);

	dispatch(static_cast<tree::Operation *>(binaryOperation));
}

void operation::Restructure::dispatch(tree::UnaryOperation *unaryOperation)
{
	tree::Expression *expression = nullptr;

	RESTRUCTURE_GET(expression, tree::Expression, unaryOperation->getExpression());
	unaryOperation->setExpression(expression);

	dispatch(static_cast<tree::Operation *>(unaryOperation));
}

void operation::Restructure::dispatch(tree::Scope *scope)
{
	dispatch(static_cast<tree::Statement *>(scope));

	tree::Statements *statements = scope->getStatements();

	if(statements)
	{
		tree::Scope *oldScope = mCurrentScope;

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

		mCurrentScope = oldScope;
	}
}

void operation::Restructure::dispatch(tree::Scoping *scoping)
{
	tree::Statement *statement = nullptr;

	RESTRUCTURE_GET(statement, tree::Statement, scoping->getScoped());
	scoping->setScoped(statement);

	dispatch(static_cast<tree::Statement *>(scoping));
}

void operation::Restructure::dispatch(tree::SizedType *sizedType)
{
	tree::Expression *size = nullptr;

	RESTRUCTURE_GET(size, tree::Expression, sizedType->getSize());
	sizedType->setSize(size);

	dispatch(static_cast<tree::Type *>(sizedType));
}

void operation::Restructure::dispatch(tree::Assign *assign)
{
	tree::Expression *lhs = nullptr;
	tree::Expression *rhs = nullptr;

	RESTRUCTURE_GET(lhs, tree::Expression, assign->getLHS());
	assign->setLHS(lhs);

	RESTRUCTURE_GET(rhs, tree::Expression, assign->getRHS());
	assign->setRHS(rhs);

	dispatch(static_cast<tree::Expression *>(assign));
}

void operation::Restructure::dispatch(tree::Cast *cast)
{
	tree::Expression *expression = nullptr;

	RESTRUCTURE_GET(expression, tree::Expression, cast->getExpression());
	cast->setExpression(expression);

	dispatch(static_cast<tree::Expression *>(cast));
}

void operation::Restructure::dispatch(tree::ComputedArray *computedArray)
{
	tree::Expression *from = nullptr;
	tree::Expression *to = nullptr;
	tree::Expression *step = nullptr;

	RESTRUCTURE_GET(step, tree::Expression, computedArray->getStep());
	computedArray->setStep(step);

	RESTRUCTURE_GET(to, tree::Expression, computedArray->getTo());
	computedArray->setTo(to);

	RESTRUCTURE_GET(from, tree::Expression, computedArray->getFrom());
	computedArray->setFrom(from);

	dispatch(static_cast<tree::Expression *>(computedArray));
}

void operation::Restructure::dispatch(tree::FunctionCall *functionCall)
{
	tree::FunctionPrototype *functionPrototype = nullptr;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, functionCall->getPrototype());
	functionCall->setPrototype(functionPrototype);

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

void operation::Restructure::dispatch(tree::IfExpression *ifExpression)
{
	tree::Expression *mFalseResult = nullptr;
	tree::Expression *mTrueResult = nullptr;
	tree::Expression *mTest = nullptr;

	RESTRUCTURE_GET(mFalseResult, tree::Expression, ifExpression->getFalseResult());
	ifExpression->setFalseResult(mFalseResult);

	RESTRUCTURE_GET(mTrueResult, tree::Expression, ifExpression->getTrueResult());
	ifExpression->setTrueResult(mTrueResult);

	RESTRUCTURE_GET(mTest, tree::Expression, ifExpression->getTest());
	ifExpression->setTest(mTest);

	dispatch(static_cast<tree::Expression *>(ifExpression));
}

void operation::Restructure::dispatch(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = nullptr;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, function->getPrototype());
	function->setPrototype(functionPrototype);

	dispatch(static_cast<tree::Scope *>(function));

	processFunctionParameters(function);
}


void operation::Restructure::dispatch(tree::Method *method)
{
	tree::Type *type = nullptr;

	RESTRUCTURE_GET(type, tree::Type, method->getType());
	method->setType(type);

	dispatch(static_cast<tree::Function *>(method));
}

void operation::Restructure::dispatch(tree::If *ifStatement)
{
	tree::Statement *falseStatement = nullptr;
	tree::Statement *trueStatement = nullptr;
	tree::Expression *test = nullptr;

	RESTRUCTURE_GET(falseStatement, tree::Statement, ifStatement->getFalseStatement());
	ifStatement->setFalseStatement(falseStatement);

	RESTRUCTURE_GET(trueStatement, tree::Statement, ifStatement->getTrueStatement());
	ifStatement->setTrueStatement(trueStatement);

	RESTRUCTURE_GET(test, tree::Expression, ifStatement->getTest());
	ifStatement->setTest(test);

	dispatch(static_cast<tree::Statement *>(ifStatement));
}

void operation::Restructure::dispatch(tree::Import *import)
{
	tree::FunctionPrototype *functionPrototype = nullptr;

	RESTRUCTURE_GET(functionPrototype, tree::FunctionPrototype, import->getPrototype());
	import->setPrototype(functionPrototype);

	dispatch(static_cast<tree::Statement *>(import));

	if(functionPrototype)
	{
		tree::Expressions *expressions = functionPrototype->getArguments();
		//tree::Scope *oldScope = mCurrentScope;

		//mCurrentScope = nullptr;

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

		//mCurrentScope = oldScope;
	}
}

void operation::Restructure::dispatch(tree::Execute *execute)
{
	tree::Expression *expression = nullptr;

	RESTRUCTURE_GET(expression, tree::Expression, execute->getExpression());
	execute->setExpression(expression);

	dispatch(static_cast<tree::Statement *>(execute));
}

void operation::Restructure::dispatch(tree::Return *opReturn)
{
	tree::Expression *expression = nullptr;

	RESTRUCTURE_GET(expression, tree::Expression, opReturn->getReturn());
	opReturn->setReturn(expression);

	dispatch(static_cast<tree::Statement *>(opReturn));
}

void operation::Restructure::dispatch(tree::TypeDefinition *typeDefinition)
{
	tree::Type *type = nullptr;

	RESTRUCTURE_GET(type, tree::Type, typeDefinition->getType());
	typeDefinition->setType(type);

	dispatch(static_cast<tree::Statement *>(typeDefinition));
}

void operation::Restructure::dispatch(tree::For *forStatement)
{
	tree::Variable *variable = nullptr;
	tree::Expression *iterable = nullptr;

	RESTRUCTURE_GET(iterable, tree::Expression, forStatement->getIterable());
	forStatement->setIterable(iterable);

	RESTRUCTURE_GET(variable, tree::Variable, forStatement->getVariable());
	forStatement->setVariable(variable);

	dispatch(static_cast<tree::Scope *>(forStatement));
}

void operation::Restructure::dispatch(tree::While *whileStatement)
{
	tree::Statement *loopStatement = nullptr;
	tree::Expression *test = nullptr;

	RESTRUCTURE_GET(loopStatement, tree::Statement, whileStatement->getLoopStatement());
	whileStatement->setLoopStatement(loopStatement);

	RESTRUCTURE_GET(test, tree::Expression, whileStatement->getTest());
	whileStatement->setTest(test);

	dispatch(static_cast<tree::Statement *>(whileStatement));
}

void operation::Restructure::dispatch(tree::UDT *udt)
{
	dispatch(static_cast<tree::Type *>(udt));

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
}

void operation::Restructure::dispatch(tree::Array *array)
{
	tree::Type *type = nullptr;

	RESTRUCTURE_GET(type, tree::Type, array->getType());
	array->setType(type);

	dispatch(static_cast<tree::SizedType *>(array));
}

void operation::Restructure::processFunctionParameters(tree::Function *function)
{
	tree::FunctionPrototype *functionPrototype = function->getPrototype();

	if(functionPrototype)
	{
		tree::Expressions *expressions = functionPrototype->getArguments();
		tree::Scope *oldScope = mCurrentScope;

		mCurrentScope = function;

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

		mCurrentScope = oldScope;
	}
}

#ifndef COMPILER_OPERATIONS_OPERATION_H
#define COMPILER_OPERATIONS_OPERATION_H


namespace operation
{
	class Operation
	{
	public:
		/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

		virtual void process(tree::Node *node) {}

		virtual void process(tree::Expression *expression)
		{
			process(static_cast<tree::Node *>(expression));
		}

		virtual void process(tree::Statement *statement)
		{
			process(static_cast<tree::Node *>(statement));
		}

		virtual void process(tree::Scope *scope)
		{
			process(static_cast<tree::Statement *>(scope));
		}

		virtual void process(tree::Identity *identity)
		{
			process(static_cast<tree::Expression *>(identity));
		}

		virtual void process(tree::TypedIdentity *typedIdentity)
		{
			process(static_cast<tree::Identity *>(typedIdentity));
		}

		virtual void process(tree::Access *access)
		{
			process(static_cast<tree::Expression *>(access));
		}

		// LITERAL???

		virtual void process(tree::UnaryExpression *unaryExpression)
		{
			process(static_cast<tree::Expression *>(unaryExpression));
		}

		virtual void process(tree::BinaryExpression *binaryExpression)
		{
			process(static_cast<tree::Expression *>(binaryExpression));
		}

		virtual void process(tree::Scoping *scoping)
		{
			process(static_cast<tree::Expression *>(scoping));
		}

		virtual void process(tree::State *state)
		{
			process(static_cast<tree::Node *>(state));
		}

		virtual void process(tree::Type *type)
		{
			process(static_cast<tree::Node *>(type));
		}

		/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

		virtual void process(tree::Identifier *identifier)
		{
			process(static_cast<tree::Expression *>(identifier));
		}

		virtual void process(tree::Constant *constant)
		{
			process(static_cast<tree::TypedIdentity *>(constant));
		}

		virtual void process(tree::Variable *variable)
		{
			process(static_cast<tree::TypedIdentity *>(variable));
		}

		virtual void process(tree::Reference *reference)
		{
			process(static_cast<tree::TypedIdentity *>(reference));
		}

		virtual void process(tree::Cast *cast)
		{
			process(static_cast<tree::Expression *>(cast));
		}

		virtual void process(tree::DirectAccess *directAccess)
		{
			process(static_cast<tree::Access *>(directAccess));
		}

		virtual void process(tree::MessageAccess *messageAccess)
		{
			process(static_cast<tree::Access *>(messageAccess));
		}

		virtual void process(tree::ArrayAccess *arrayAccess)
		{
			process(static_cast<tree::Access *>(arrayAccess));
		}

		virtual void process(tree::FunctionCall *functionCall)
		{
			process(static_cast<tree::Expression *>(functionCall));
		}

		virtual void process(tree::NullReference *nullReference)
		{
			process(static_cast<tree::Expression *>(nullReference));
		}

		virtual void process(tree::BoolLiteral *boolLiteral)
		{
			process(static_cast<tree::Expression *>(boolLiteral)); // FIXME
		}

		virtual void process(tree::IntLiteral *intLiteral)
		{
			process(static_cast<tree::Expression *>(intLiteral)); // FIXME
		}

		virtual void process(tree::FloatLiteral *floatLiteral)
		{
			process(static_cast<tree::Expression *>(floatLiteral)); // FIXME
		}

		virtual void process(tree::StringLiteral *stringLiteral)
		{
			process(static_cast<tree::Expression *>(stringLiteral)); // FIXME
		}

		virtual void process(tree::Assign *assign)
		{
			process(static_cast<tree::BinaryExpression *>(assign));
		}

		virtual void process(tree::LogicalOr *logicalOr)
		{
			process(static_cast<tree::BinaryExpression *>(logicalOr));
		}

		virtual void process(tree::LogicalAnd *logicalAnd)
		{
			process(static_cast<tree::BinaryExpression *>(logicalAnd));
		}

		virtual void process(tree::Or *nOr)
		{
			process(static_cast<tree::BinaryExpression *>(nOr));
		}

		virtual void process(tree::Xor *nXor)
		{
			process(static_cast<tree::BinaryExpression *>(nXor));
		}

		virtual void process(tree::And *nAnd)
		{
			process(static_cast<tree::BinaryExpression *>(nAnd));
		}

		virtual void process(tree::Equal *equal)
		{
			process(static_cast<tree::BinaryExpression *>(equal));
		}

		virtual void process(tree::Unequal *unequal)
		{
			process(static_cast<tree::BinaryExpression *>(unequal));
		}

		virtual void process(tree::LessThan *lessThan)
		{
			process(static_cast<tree::BinaryExpression *>(lessThan));
		}

		virtual void process(tree::LessEqual *lessEqual)
		{
			process(static_cast<tree::BinaryExpression *>(lessEqual));
		}

		virtual void process(tree::GreaterThan *greaterThan)
		{
			process(static_cast<tree::BinaryExpression *>(greaterThan));
		}

		virtual void process(tree::GreaterEqual *greaterEqual)
		{
			process(static_cast<tree::BinaryExpression *>(greaterEqual));
		}

		virtual void process(tree::Add *add)
		{
			process(static_cast<tree::BinaryExpression *>(add));
		}

		virtual void process(tree::Subtract *subtract)
		{
			process(static_cast<tree::BinaryExpression *>(subtract));
		}

		virtual void process(tree::Multiply *multiply)
		{
			process(static_cast<tree::BinaryExpression *>(multiply));
		}

		virtual void process(tree::Divide *divide)
		{
			process(static_cast<tree::BinaryExpression *>(divide));
		}

		virtual void process(tree::Modulus *modulus)
		{
			process(static_cast<tree::BinaryExpression *>(modulus));
		}

		virtual void process(tree::LogicalNot *logicalNot)
		{
			process(static_cast<tree::UnaryExpression *>(logicalNot));
		}

		virtual void process(tree::Not *nNot)
		{
			process(static_cast<tree::UnaryExpression *>(nNot));
		}

		virtual void process(tree::Minus *minus)
		{
			process(static_cast<tree::UnaryExpression *>(minus));
		}

		virtual void process(tree::Execute *execute)
		{
			process(static_cast<tree::Statement *>(execute));
		}

		virtual void process(tree::Return *nReturn)
		{
			process(static_cast<tree::Statement *>(nReturn));
		}

		virtual void process(tree::SetState *setState)
		{
			process(static_cast<tree::Statement *>(setState));
		}

		virtual void process(tree::FunctionPrototype *functionPrototype)
		{
			process(static_cast<tree::TypedIdentity *>(functionPrototype));
		}

		virtual void process(tree::Function *function)
		{
			process(static_cast<tree::Scope *>(function));
		}

		virtual void process(tree::Program *program)
		{
			process(static_cast<tree::Scope *>(program));
		}

		virtual void process(tree::GlobalScoping *globalScoping)
		{
			process(static_cast<tree::Scoping *>(globalScoping));
		}

		virtual void process(tree::SharedScoping *sharedScoping)
		{
			process(static_cast<tree::Scoping *>(sharedScoping));
		}

		virtual void process(tree::DefaultState *defaultState)
		{
			process(static_cast<tree::State *>(defaultState));
		}

		virtual void process(tree::NamedState *namedState)
		{
			process(static_cast<tree::State *>(namedState));
		}

		virtual void process(tree::Bool *nBool)
		{
			process(static_cast<tree::Type *>(nBool));
		}

		virtual void process(tree::Int *nInt)
		{
			process(static_cast<tree::Type *>(nInt));
		}

		virtual void process(tree::Float *nFloat)
		{
			process(static_cast<tree::Type *>(nFloat));
		}

		virtual void process(tree::String *string)
		{
			process(static_cast<tree::Type *>(string));
		}

		virtual void process(tree::UDT *udt)
		{
			process(static_cast<tree::Type *>(udt)); // FIXME
		}

		virtual void process(tree::Use *use)
		{
			process(static_cast<tree::Scope *>(use));
		}

	protected:
		Operation() { /* Can't directly instantiate */ }
	};
}

#endif

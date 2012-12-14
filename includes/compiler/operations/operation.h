#ifndef COMPILER_OPERATIONS_OPERATION_H
#define COMPILER_OPERATIONS_OPERATION_H


// This is rather ugly but it saves a LOT of typing...
#define PROCESS_ACTIONS(base_class, class) \
	virtual void setup(class *node) \
	{ \
		setup(static_cast<base_class *>(node)); \
	} \
	virtual void visit(class *node) \
	{ \
		visit(static_cast<base_class *>(node)); \
	}

namespace operation
{
	class Operation
	{
	public:
		virtual void setup(tree::Node *node) {}
		virtual void visit(tree::Node *node) {}

		/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

		PROCESS_ACTIONS(tree::Node, tree::Expression)
		PROCESS_ACTIONS(tree::Node, tree::Statement)
		PROCESS_ACTIONS(tree::Node, tree::State)
		PROCESS_ACTIONS(tree::Node, tree::Type)

		PROCESS_ACTIONS(tree::Statement, tree::Scope)

		PROCESS_ACTIONS(tree::Expression, tree::Access)
		PROCESS_ACTIONS(tree::Expression, tree::BinaryExpression)
		PROCESS_ACTIONS(tree::Expression, tree::Identity)
		PROCESS_ACTIONS(tree::Expression, tree::Scoping)
		PROCESS_ACTIONS(tree::Expression, tree::UnaryExpression)

		PROCESS_ACTIONS(tree::Identity, tree::TypedIdentity)

		/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

		PROCESS_ACTIONS(tree::Access, tree::ArrayAccess)
		PROCESS_ACTIONS(tree::Access, tree::DirectAccess)
		PROCESS_ACTIONS(tree::Access, tree::MessageAccess)

		PROCESS_ACTIONS(tree::BinaryExpression, tree::Add)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::And)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Assign)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Divide)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Equal)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::GreaterEqual)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::GreaterThan)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::LessEqual)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::LessThan)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::LogicalAnd)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::LogicalOr)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Modulus)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Multiply)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Or)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Subtract)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Unequal)
		PROCESS_ACTIONS(tree::BinaryExpression, tree::Xor)

		PROCESS_ACTIONS(tree::Expression, tree::BoolLiteral)
		PROCESS_ACTIONS(tree::Expression, tree::IntLiteral)
		PROCESS_ACTIONS(tree::Expression, tree::FloatLiteral)
		PROCESS_ACTIONS(tree::Expression, tree::StringLiteral)

		PROCESS_ACTIONS(tree::Expression, tree::Cast)
		PROCESS_ACTIONS(tree::Expression, tree::FunctionCall)
		PROCESS_ACTIONS(tree::Expression, tree::Identifier)
		PROCESS_ACTIONS(tree::Expression, tree::NullReference)

		PROCESS_ACTIONS(tree::Scope, tree::Function)
		PROCESS_ACTIONS(tree::Scope, tree::Program)
		PROCESS_ACTIONS(tree::Scope, tree::Use)

		PROCESS_ACTIONS(tree::Scoping, tree::GlobalScoping)
		PROCESS_ACTIONS(tree::Scoping, tree::SharedScoping)

		PROCESS_ACTIONS(tree::State, tree::DefaultState)
		PROCESS_ACTIONS(tree::State, tree::NamedState)

		PROCESS_ACTIONS(tree::Statement, tree::Execute)
		PROCESS_ACTIONS(tree::Statement, tree::Return)
		PROCESS_ACTIONS(tree::Statement, tree::SetState)

		PROCESS_ACTIONS(tree::Type, tree::Bool)
		PROCESS_ACTIONS(tree::Type, tree::Float)
		PROCESS_ACTIONS(tree::Type, tree::Int)
		PROCESS_ACTIONS(tree::Type, tree::String)
		PROCESS_ACTIONS(tree::Type, tree::UDT) // FIXME

		PROCESS_ACTIONS(tree::TypedIdentity, tree::Constant)
		PROCESS_ACTIONS(tree::TypedIdentity, tree::FunctionPrototype)
		PROCESS_ACTIONS(tree::TypedIdentity, tree::Reference)
		PROCESS_ACTIONS(tree::TypedIdentity, tree::Variable)

		PROCESS_ACTIONS(tree::UnaryExpression, tree::LogicalNot)
		PROCESS_ACTIONS(tree::UnaryExpression, tree::Not)
		PROCESS_ACTIONS(tree::UnaryExpression, tree::Minus)

	protected:
		Operation() { /* Can't directly instantiate */ }
	};
}

#endif

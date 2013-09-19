#ifndef COMPILER_OPERATIONS_OPERATION_H
#define COMPILER_OPERATIONS_OPERATION_H

#include "compiler/tree.h"

namespace operation
{
	// This is rather ugly but it saves a LOT of typing...
	#define PROCESS_ACTION(base_class, class, return_type, name) \
		virtual return_type name(class *node) \
		{ \
			return name(static_cast<base_class *>(node)); \
		}

	#define PROCESS_ACTIONS(return_type, name) \
		/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */ \
		 \
		PROCESS_ACTION(tree::Node, tree::Expression, return_type, name) \
		PROCESS_ACTION(tree::Node, tree::Statement, return_type, name) \
		/*PROCESS_ACTION(tree::Node, tree::State, return_type, name)*/ \
		PROCESS_ACTION(tree::Node, tree::Type, return_type, name) \
		 \
		PROCESS_ACTION(tree::Expression, tree::Access, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::Identity, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::Literal, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::Operation, return_type, name) \
		 \
		PROCESS_ACTION(tree::Identity, tree::TypedIdentity, return_type, name) \
		 \
		PROCESS_ACTION(tree::Literal, tree::NumericLiteral, return_type, name) \
		 \
		PROCESS_ACTION(tree::Operation, tree::BinaryOperation, return_type, name) \
		PROCESS_ACTION(tree::Operation, tree::UnaryOperation, return_type, name) \
		 \
		PROCESS_ACTION(tree::BinaryOperation, tree::Assign, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::BooleanBinaryOperation, return_type, name) \
		 \
		PROCESS_ACTION(tree::UnaryOperation, tree::BooleanUnaryOperation, return_type, name) \
		 \
		PROCESS_ACTION(tree::Statement, tree::Scope, return_type, name) \
		PROCESS_ACTION(tree::Statement, tree::Scoping, return_type, name) \
		 \
		PROCESS_ACTION(tree::Scope, tree::NamedScope, return_type, name) \
		 \
		PROCESS_ACTION(tree::Type, tree::SizedType, return_type, name) \
		 \
		/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */ \
		 \
		PROCESS_ACTION(tree::Node, tree::Identifier, return_type, name) \
		 \
		PROCESS_ACTION(tree::Access, tree::ArrayAccess, return_type, name) \
		PROCESS_ACTION(tree::Access, tree::DirectAccess, return_type, name) \
		PROCESS_ACTION(tree::Access, tree::MessageAccess, return_type, name) \
		 \
		PROCESS_ACTION(tree::TypedIdentity, tree::Constant, return_type, name) \
		PROCESS_ACTION(tree::TypedIdentity, tree::FunctionPrototype, return_type, name) \
		PROCESS_ACTION(tree::TypedIdentity, tree::Member, return_type, name) \
		PROCESS_ACTION(tree::TypedIdentity, tree::Variable, return_type, name) \
		 \
		PROCESS_ACTION(tree::BinaryOperation, tree::Add, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::And, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Divide, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Modulus, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Multiply, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Or, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Subtract, return_type, name) \
		PROCESS_ACTION(tree::BinaryOperation, tree::Xor, return_type, name) \
		 \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::Equal, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::GreaterEqual, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::GreaterThan, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::LessEqual, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::LessThan, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::LogicalAnd, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::LogicalOr, return_type, name) \
		PROCESS_ACTION(tree::BooleanBinaryOperation, tree::Unequal, return_type, name) \
		 \
		PROCESS_ACTION(tree::BooleanUnaryOperation, tree::LogicalNot, return_type, name) \
		 \
		PROCESS_ACTION(tree::Expression, tree::Cast, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::FunctionCall, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::IfExpression, return_type, name) \
		PROCESS_ACTION(tree::Expression, tree::This, return_type, name) \
		 \
		PROCESS_ACTION(tree::Literal, tree::BoolLiteral, return_type, name) \
		PROCESS_ACTION(tree::Literal, tree::StringLiteral, return_type, name) \
		 \
		PROCESS_ACTION(tree::NumericLiteral, tree::IntLiteral, return_type, name) \
		PROCESS_ACTION(tree::NumericLiteral, tree::FloatLiteral, return_type, name) \
		 \
		PROCESS_ACTION(tree::NamedScope, tree::Program, return_type, name) \
		PROCESS_ACTION(tree::NamedScope, tree::Use, return_type, name) \
		 \
		PROCESS_ACTION(tree::Scope, tree::Aggregate, return_type, name) \
		PROCESS_ACTION(tree::Scope, tree::AnonymousScope, return_type, name) \
		PROCESS_ACTION(tree::Scope, tree::Function, return_type, name) \
		 \
		PROCESS_ACTION(tree::Function, tree::Method, return_type, name) \
		 \
		PROCESS_ACTION(tree::Scoping, tree::GlobalScoping, return_type, name) \
		PROCESS_ACTION(tree::Scoping, tree::SharedScoping, return_type, name) \
		 \
		/*PROCESS_ACTION(tree::State, tree::DefaultState, return_type, name)*/ \
		/*PROCESS_ACTION(tree::State, tree::NamedState, return_type, name)*/ \
		 \
		PROCESS_ACTION(tree::Statement, tree::If, return_type, name) \
		PROCESS_ACTION(tree::Statement, tree::Import, return_type, name) \
		PROCESS_ACTION(tree::Statement, tree::Execute, return_type, name) \
		PROCESS_ACTION(tree::Statement, tree::Return, return_type, name) \
		/*PROCESS_ACTION(tree::Statement, tree::SetState, return_type, name)*/ \
		PROCESS_ACTION(tree::Statement, tree::TypeDefinition, return_type, name) \
		PROCESS_ACTION(tree::Statement, tree::While, return_type, name) \
		 \
		PROCESS_ACTION(tree::Type, tree::Bool, return_type, name) \
		PROCESS_ACTION(tree::Type, tree::UDT, return_type, name) \
		PROCESS_ACTION(tree::Type, tree::Void, return_type, name) \
		 \
		PROCESS_ACTION(tree::SizedType, tree::Array, return_type, name) \
		PROCESS_ACTION(tree::SizedType, tree::Float, return_type, name) \
		PROCESS_ACTION(tree::SizedType, tree::Int, return_type, name) \
		PROCESS_ACTION(tree::SizedType, tree::String, return_type, name) \
		 \
		PROCESS_ACTION(tree::UnaryOperation, tree::Not, return_type, name) \
		PROCESS_ACTION(tree::UnaryOperation, tree::Minus, return_type, name)

	class Operation
	{
	public:
		virtual void setup(tree::Node *node) {}
		PROCESS_ACTIONS(void, setup)

		virtual bool doProcessChildren(tree::Node *node) { return true; }
		PROCESS_ACTIONS(bool, doProcessChildren)

		virtual void visit(tree::Node *node) {}
		PROCESS_ACTIONS(void, visit)

	protected:
		Operation() { /* Abstract class */ }
	};
}

#endif

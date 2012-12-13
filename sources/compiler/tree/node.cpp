#include "compiler/operation.h"
#include "compiler/tree.h"


// This is rather ugly but it saves a LOT of typing...
#define DISPATCH_ACTIONS(class, function) \
	if(dynamic_cast<class *>(this)) \
	{ \
		return function(static_cast<class *>(this)); \
	}

#define GENERATE_DISPATCH(function) \
	DISPATCH_ACTIONS(tree::Add, function) \
	DISPATCH_ACTIONS(tree::And, function) \
	DISPATCH_ACTIONS(tree::ArrayAccess, function) \
	DISPATCH_ACTIONS(tree::Assign, function) \
	DISPATCH_ACTIONS(tree::Bool, function) \
	DISPATCH_ACTIONS(tree::BoolLiteral, function) \
	DISPATCH_ACTIONS(tree::Cast, function) \
	DISPATCH_ACTIONS(tree::Constant, function) \
	DISPATCH_ACTIONS(tree::DefaultState, function) \
	DISPATCH_ACTIONS(tree::DirectAccess, function) \
	DISPATCH_ACTIONS(tree::Divide, function) \
	DISPATCH_ACTIONS(tree::Equal, function) \
	DISPATCH_ACTIONS(tree::Execute, function) \
	DISPATCH_ACTIONS(tree::Float, function) \
	DISPATCH_ACTIONS(tree::FloatLiteral, function) \
	DISPATCH_ACTIONS(tree::Function, function) \
	DISPATCH_ACTIONS(tree::FunctionCall, function) \
	DISPATCH_ACTIONS(tree::FunctionPrototype, function) \
	DISPATCH_ACTIONS(tree::GlobalScoping, function) \
	DISPATCH_ACTIONS(tree::GreaterEqual, function) \
	DISPATCH_ACTIONS(tree::GreaterThan, function) \
	DISPATCH_ACTIONS(tree::Identifier, function) \
	DISPATCH_ACTIONS(tree::Int, function) \
	DISPATCH_ACTIONS(tree::IntLiteral, function) \
	DISPATCH_ACTIONS(tree::LessEqual, function) \
	DISPATCH_ACTIONS(tree::LessThan, function) \
	DISPATCH_ACTIONS(tree::LogicalAnd, function) \
	DISPATCH_ACTIONS(tree::LogicalNot, function) \
	DISPATCH_ACTIONS(tree::LogicalOr, function) \
	DISPATCH_ACTIONS(tree::MessageAccess, function) \
	DISPATCH_ACTIONS(tree::Minus, function) \
	DISPATCH_ACTIONS(tree::Modulus, function) \
	DISPATCH_ACTIONS(tree::Multiply, function) \
	DISPATCH_ACTIONS(tree::NamedState, function) \
	DISPATCH_ACTIONS(tree::Not, function) \
	DISPATCH_ACTIONS(tree::NullReference, function) \
	DISPATCH_ACTIONS(tree::Or, function) \
	DISPATCH_ACTIONS(tree::Program, function) \
	DISPATCH_ACTIONS(tree::Reference, function) \
	DISPATCH_ACTIONS(tree::Return, function) \
	DISPATCH_ACTIONS(tree::SetState, function) \
	DISPATCH_ACTIONS(tree::SharedScoping, function) \
	DISPATCH_ACTIONS(tree::String, function) \
	DISPATCH_ACTIONS(tree::StringLiteral, function) \
	DISPATCH_ACTIONS(tree::Subtract, function) \
	DISPATCH_ACTIONS(tree::UDT, function) \
	DISPATCH_ACTIONS(tree::Unequal, function) \
	DISPATCH_ACTIONS(tree::Use, function) \
	DISPATCH_ACTIONS(tree::Variable, function) \
	DISPATCH_ACTIONS(tree::Xor, function)

void tree::Node::dispatch(operation::Operation *operation)
{
	// We only need to call these on the concrete classes!

	GENERATE_DISPATCH(operation->process)
	ERROR("Unknown concrete class");
}

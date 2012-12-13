#include "compiler/operation.h"
#include "compiler/tree.h"


void tree::Node::dispatch(operation::Operation *operation)
{
	// We only need to call these on the concrete classes!

	if(dynamic_cast<tree::Identifier *>(this))
	{
		operation->process(static_cast<tree::Identifier *>(this));
	}
	else if(dynamic_cast<tree::Constant *>(this))
	{
		operation->process(static_cast<tree::Constant *>(this));
	}
	else if(dynamic_cast<tree::Variable *>(this))
	{
		operation->process(static_cast<tree::Variable *>(this));
	}
	else if(dynamic_cast<tree::Reference *>(this))
	{
		operation->process(static_cast<tree::Reference *>(this));
	}
	else if(dynamic_cast<tree::Cast *>(this))
	{
		operation->process(static_cast<tree::Cast *>(this));
	}
	else if(dynamic_cast<tree::DirectAccess *>(this))
	{
		operation->process(static_cast<tree::DirectAccess *>(this));
	}
	else if(dynamic_cast<tree::MessageAccess *>(this))
	{
		operation->process(static_cast<tree::MessageAccess *>(this));
	}
	else if(dynamic_cast<tree::ArrayAccess *>(this))
	{
		operation->process(static_cast<tree::ArrayAccess *>(this));
	}
	else if(dynamic_cast<tree::FunctionCall *>(this))
	{
		operation->process(static_cast<tree::FunctionCall *>(this));
	}
	else if(dynamic_cast<tree::IntLiteral *>(this))
	{
		operation->process(static_cast<tree::IntLiteral *>(this));
	}
	else if(dynamic_cast<tree::FloatLiteral *>(this))
	{
		operation->process(static_cast<tree::FloatLiteral *>(this));
	}
	else if(dynamic_cast<tree::StringLiteral *>(this))
	{
		operation->process(static_cast<tree::StringLiteral *>(this));
	}
	else if(dynamic_cast<tree::Assign *>(this))
	{
		operation->process(static_cast<tree::Assign *>(this));
	}
	else if(dynamic_cast<tree::LogicalOr *>(this))
	{
		operation->process(static_cast<tree::LogicalOr *>(this));
	}
	else if(dynamic_cast<tree::LogicalAnd *>(this))
	{
		operation->process(static_cast<tree::LogicalAnd *>(this));
	}
	else if(dynamic_cast<tree::Or *>(this))
	{
		operation->process(static_cast<tree::Or *>(this));
	}
	else if(dynamic_cast<tree::Xor *>(this))
	{
		operation->process(static_cast<tree::Xor *>(this));
	}
	else if(dynamic_cast<tree::And *>(this))
	{
		operation->process(static_cast<tree::And *>(this));
	}
	else if(dynamic_cast<tree::Equal *>(this))
	{
		operation->process(static_cast<tree::Equal *>(this));
	}
	else if(dynamic_cast<tree::Unequal *>(this))
	{
		operation->process(static_cast<tree::Unequal *>(this));
	}
	else if(dynamic_cast<tree::LessThan *>(this))
	{
		operation->process(static_cast<tree::LessThan *>(this));
	}
	else if(dynamic_cast<tree::LessEqual *>(this))
	{
		operation->process(static_cast<tree::LessEqual *>(this));
	}
	else if(dynamic_cast<tree::GreaterThan *>(this))
	{
		operation->process(static_cast<tree::GreaterThan *>(this));
	}
	else if(dynamic_cast<tree::GreaterEqual *>(this))
	{
		operation->process(static_cast<tree::GreaterEqual *>(this));
	}
	else if(dynamic_cast<tree::Add *>(this))
	{
		operation->process(static_cast<tree::Add *>(this));
	}
	else if(dynamic_cast<tree::Subtract *>(this))
	{
		operation->process(static_cast<tree::Subtract *>(this));
	}
	else if(dynamic_cast<tree::Multiply *>(this))
	{
		operation->process(static_cast<tree::Multiply *>(this));
	}
	else if(dynamic_cast<tree::Divide *>(this))
	{
		operation->process(static_cast<tree::Divide *>(this));
	}
	else if(dynamic_cast<tree::Modulus *>(this))
	{
		operation->process(static_cast<tree::Modulus *>(this));
	}
	else if(dynamic_cast<tree::LogicalNot *>(this))
	{
		operation->process(static_cast<tree::LogicalNot *>(this));
	}
	else if(dynamic_cast<tree::Not *>(this))
	{
		operation->process(static_cast<tree::Not *>(this));
	}
	else if(dynamic_cast<tree::Minus *>(this))
	{
		operation->process(static_cast<tree::Minus *>(this));
	}
	else if(dynamic_cast<tree::Execute *>(this))
	{
		operation->process(static_cast<tree::Execute *>(this));
	}
	else if(dynamic_cast<tree::Return *>(this))
	{
		operation->process(static_cast<tree::Return *>(this));
	}
	else if(dynamic_cast<tree::SetState *>(this))
	{
		operation->process(static_cast<tree::SetState *>(this));
	}
	else if(dynamic_cast<tree::FunctionPrototype *>(this))
	{
		operation->process(static_cast<tree::FunctionPrototype *>(this));
	}
	else if(dynamic_cast<tree::Function *>(this))
	{
		operation->process(static_cast<tree::Function *>(this));
	}
	else if(dynamic_cast<tree::Program *>(this))
	{
		operation->process(static_cast<tree::Program *>(this));
	}
	else if(dynamic_cast<tree::GlobalScoping *>(this))
	{
		operation->process(static_cast<tree::GlobalScoping *>(this));
	}
	else if(dynamic_cast<tree::SharedScoping *>(this))
	{
		operation->process(static_cast<tree::SharedScoping *>(this));
	}
	else if(dynamic_cast<tree::DefaultState *>(this))
	{
		operation->process(static_cast<tree::DefaultState *>(this));
	}
	else if(dynamic_cast<tree::NamedState *>(this))
	{
		operation->process(static_cast<tree::NamedState *>(this));
	}
	else if(dynamic_cast<tree::Int *>(this))
	{
		operation->process(static_cast<tree::Int *>(this));
	}
	else if(dynamic_cast<tree::Float *>(this))
	{
		operation->process(static_cast<tree::Float *>(this));
	}
	else if(dynamic_cast<tree::String *>(this))
	{
		operation->process(static_cast<tree::String *>(this));
	}
	else if(dynamic_cast<tree::UDT *>(this))
	{
		operation->process(static_cast<tree::UDT *>(this));
	}
	else if(dynamic_cast<tree::Use *>(this))
	{
		operation->process(static_cast<tree::Use *>(this));
	}
	else
	{
		ERROR("Unknown concrete class!");
	}
}

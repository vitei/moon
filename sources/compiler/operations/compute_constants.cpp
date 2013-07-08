#include "compiler/operations.h"
#include "compiler/tree.h"


void operation::ComputeConstants::run(tree::Program *program)
{
	operation::ComputeConstants operation;
	program->accept(&operation);
}

tree::Node *operation::ComputeConstants::restructure(tree::Cast *cast)
{
	LOG("ComputeConstants::restructure::Cast");

	tree::Literal *literal = dynamic_cast<tree::Literal *>(cast->getExpression());

	if(literal)
	{
		tree::Type *type = cast->getType();

		if(dynamic_cast<tree::Bool *>(type))
		{
			return new tree::BoolLiteral(literal);
		}
		else if(dynamic_cast<tree::Int *>(type))
		{
			return new tree::IntLiteral(literal);
		}
		else if(dynamic_cast<tree::Float *>(type))
		{
			return new tree::FloatLiteral(literal);
		}
		/*else if(dynamic_cast<tree::String *>(type))
		{
		}*/
	}

	return cast;
}

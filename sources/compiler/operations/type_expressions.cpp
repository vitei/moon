#include "compiler/tree.h"
#include "compiler/operations/type_expressions.h"


void operation::TypeExpressions::run(tree::Program *program)
{
	operation::TypeExpressions operation;
	program->accept(&operation);
}



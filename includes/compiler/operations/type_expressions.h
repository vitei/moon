#ifndef COMPILER_OPERATIONS_TYPE_EXPRESSIONS_H
#define COMPILER_OPERATIONS_TYPE_EXPRESSIONS_H

#include "compiler/tree.h"
#include "depth_restructure.h"


namespace operation
{
	class TypeExpressions : public DepthRestructure
	{
	public:
		static void run(tree::Program *program);

		//virtual void visit(tree::Scope *scope);
		//virtual void visit(tree::GlobalScoping *globalScoping);
		//virtual void visit(tree::SharedScoping *sharedScoping);

	private:
		TypeExpressions() {}
	};
}

#endif

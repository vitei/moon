#ifndef COMPILER_TREE_NODE_H
#define COMPILER_TREE_NODE_H


namespace operation
{
	class Operation;
}

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Node
	{
	public:
		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			visit(operation);
		}

		void setup(operation::Operation *operation);
		void visit(operation::Operation *operation);

	protected:
		Node() { /* Abstract class */ }
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

}

#endif

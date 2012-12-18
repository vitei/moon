#ifndef COMPILER_TREE_NODE_H
#define COMPILER_TREE_NODE_H


namespace operation
{
	class Operation;
	class Restructure;
}

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Node
	{
	public:
		virtual void childAccept(operation::Operation *operation) {}

		virtual void accept(operation::Operation *operation)
		{
			setup(operation);
			childAccept(operation);
			visit(operation);
		}

		void setup(operation::Operation *operation);
		void visit(operation::Operation *operation);
		Node *restructure(operation::Restructure *operation);

	protected:
		Node() { /* Abstract class */ }
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

}

#endif

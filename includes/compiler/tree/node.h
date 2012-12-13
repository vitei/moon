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
		void dispatch(operation::Operation *operation);
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

}

#endif

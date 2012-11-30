#ifndef TREE_NODE_SCOPE_H
#define TREE_NODE_SCOPE_H

#include "expression.h"
#include "node.h"


namespace tree
{
	class NodeScope : public Expression
	{
	public:
		NodeScope(Node *node) : mNode(node) {}

		Node *getScopedNode()
		{
			return mNode;
		}

	private:
		Node *mNode;
	};

	class GlobalNodeScope : public NodeScope
	{
	public:
		GlobalNodeScope(Node *node) : NodeScope(node) {}
	};

	class SharedNodeScope : public NodeScope
	{
	public:
		SharedNodeScope(Node *node) : NodeScope(node) {}
	};
}

#endif

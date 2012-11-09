#ifndef TREE_NODE_H
#define TREE_NODE_H

namespace tree
{
	class Node
	{
		void setSibling(Node *node);

	private:
		Node *mSibling;
	};
}

#endif

#ifndef TREE_NODE_H
#define TREE_NODE_H

namespace tree
{
	class Node
	{
		void setSibling(Node *node);
		void setChild(Node *node);

	private:
		Node *mParent;
		Node *mSibling;
		Node *mChild;
	};
}

#endif

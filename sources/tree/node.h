#ifndef TREE_NODE_H
#define TREE_NODE_H

namespace tree
{
	class Node
	{
	};

	class NodeList : public Node
	{
	public:
		void add(Node *node)
		{
			mList.push_back(node);
		}

		const std::vector<Node *> &getList()
		{
			return mList;
		}

	private:
		std::vector<Node *> mList;
	};
}

#endif

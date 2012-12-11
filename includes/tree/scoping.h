#ifndef TREE_SCOPING_H
#define TREE_SCOPING_H

#include "expression.h"
#include "node.h"


namespace tree
{
	class Scoping : public Expression
	{
	public:
		Scoping(Node *node) : mNode(node) {}

		Node *getNode()
		{
			return mNode;
		}

	private:
		Node *mNode;
	};

	class GlobalScoping: public Scoping
	{
	public:
		GlobalScoping(Node *node) : Scoping(node) {}
	};

	class SharedScoping : public Scoping
	{
	public:
		SharedScoping(Node *node) : Scoping(node) {}
	};
}

#endif

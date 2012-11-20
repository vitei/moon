#ifndef TREE_SCOPE_H
#define TREE_SCOPE_H

#include "expression.h"
#include "node.h"


namespace tree
{
	class Scope : public Expression
	{
	public:
		Scope(Node *node) : mNode(node) {}

	private:
		Node *mNode;
	};

	class GlobalScope : public Scope
	{
	public:
		GlobalScope(Node *node) : Scope(node) {}
	};

	class SharedScope : public Scope
	{
	public:
		SharedScope(Node *node) : Scope(node) {}
	};
}

#endif

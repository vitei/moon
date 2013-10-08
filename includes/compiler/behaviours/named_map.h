#ifndef COMPILER_BEHAVIOURS_NAMED_MAP_H
#define COMPILER_BEHAVIOURS_NAMED_MAP_H

#include <map>

namespace tree
{
	class Node;
	class Identifier;
}

namespace behaviour
{
	class NamedMap
	{
	public:
		class ExistsException : public std::exception
		{
		public:
			ExistsException(tree::Node *_node, tree::Node *_conflict) : node(_node), conflict(_conflict) {}

			tree::Node *node;
			tree::Node *conflict;
		};

		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(tree::Identifier *_identifier) : identifier(_identifier) {}

			tree::Identifier *identifier;
		};

		typedef std::map<std::string, tree::Node *> NamedNodes;
		typedef std::map<tree::Node *, NamedNodes> AssociatedNamedNodes;

		virtual void checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node) = 0;
		virtual tree::Node *findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier) = 0;
		virtual void mapAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node) = 0;

		void checkNamedNode(const std::string &name, tree::Node *node) { checkAssociatedNamedNode(nullptr, name, node); }
		tree::Node *findNamedNode(tree::Identifier *identifier) { return findAssociatedNamedNode(nullptr, identifier); }
		void mapNamedNode(const std::string &name, tree::Node *node) { mapAssociatedNamedNode(nullptr, name, node); }

	protected:
		NamedMap() { /* Abstract class */ }
	};
}

#endif

#ifndef COMPILER_TREE_SCOPE_H
#define COMPILER_TREE_SCOPE_H

#include "common.h"

#include <exception>
#include <map>
#include <string>
#include "compiler/behaviours/named_map.h"
#include "node.h"
#include "identifier.h"
#include "statement.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Scope : public Statement, public behaviour::NamedMap
	{
	public:
		Scope *getParent() const
		{
			return mParent;
		}

		void setParent(Scope *parent)
		{
			mParent = parent;
		}

		AssociatedNamedNodes &getAssociatedNamedNodes()
		{
			return mAssociatedNamedNodes;
		}

		NamedNodes &getNamedNodes()
		{
			return mAssociatedNamedNodes[nullptr];
		}

		Statements *getStatements() const
		{
			return mStatements;
		}

		virtual void checkAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node);
		virtual tree::Node *findAssociatedNamedNode(tree::Node *association, tree::Identifier *identifier);
		virtual void mapAssociatedNamedNode(tree::Node *association, const std::string &name, tree::Node *node);

	protected:
		Scope(Statements *statements) : mParent(nullptr), mStatements(statements) { /* Abstract class */ }

		Scope *mParent;
		behaviour::NamedMap::AssociatedNamedNodes mAssociatedNamedNodes;

	private:
		Statements *mStatements;
	};

	class NamedScope : public Scope
	{
	public:
		const std::string &getName() const
		{
			return mName;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("NamedScope"); }
#endif

	protected:
		NamedScope(std::string name, Statements *statements) : Scope(statements), mName(name) { /* Abstract class */ }

	private:
		std::string mName;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class AnonymousScope final : public Scope
	{
	public:
		AnonymousScope(Statements *statements) : Scope(statements) {}

#ifdef DEBUG
		virtual void printNode() { LOG("AnonymousScope"); }
#endif
	};
}

#endif

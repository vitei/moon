#ifndef COMPILER_TREE_UDT_H
#define COMPILER_TREE_UDT_H

#include <string>
#include "../behaviours/named_map.h"
#include "expression.h"
#include "node.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class UDT : public Type, public behaviour::NamedMap
	{
	public:
		class ExistsException : public std::exception
		{
		public:
			ExistsException(tree::Variable *_variable, tree::Variable *_conflict) : variable(_variable), conflict(_conflict) {}

			tree::Variable *variable;
			tree::Variable *conflict;
		};

		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(tree::Identifier *_identifier) : identifier(_identifier) {}

			tree::Identifier *identifier;
		};

		UDT(std::string name, Variables *members) : mName(name), mMembers(members) {}

		const std::string &getName() const
		{
			return mName;
		}

		virtual const char *getTypeName() const
		{
			return mName.c_str();
		}

		Variables *getMembers() const
		{
			return mMembers;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("UDT"); }
		virtual void printType() { LOG("UDT: %s", mName.c_str()); }
#endif

		virtual void checkNamedNode(const std::string &name, Node *node);
		virtual Node *findNamedNode(Identifier *identifier);
		virtual void mapNamedNode(const std::string &name, Node *node);

	protected:
		virtual bool equals(const Type &type) const;

	private:
		std::string mName;
		Variables *mMembers;
		behaviour::NamedMap::NamedNodes mNamedNodes;
	};
}

#endif

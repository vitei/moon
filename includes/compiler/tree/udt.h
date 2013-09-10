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

	class Member : public TypedIdentity
	{
	public:
		Member(Type *type, std::string name) : TypedIdentity(type, name) {}

#ifdef DEBUG
		virtual void printNode() { LOG("Member"); }
#endif
	};

	typedef std::list<Member *> Members;

	class UDT : public Type, public behaviour::NamedMap
	{
	public:
		UDT(std::string name, Members *members) : mName(name), mMembers(members) {}

		const std::string &getName() const
		{
			return mName;
		}

		virtual const char *getTypeName() const
		{
			return mName.c_str();
		}

		Members *getMembers() const
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
		Members *mMembers;
		behaviour::NamedMap::NamedNodes mNamedNodes;
	};
}

#endif

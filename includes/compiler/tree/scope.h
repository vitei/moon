#ifndef COMPILER_TREE_SCOPE_H
#define COMPILER_TREE_SCOPE_H

#include <exception>
#include <map>
#include <string>
#include "node.h"
#include "identifier.h"
#include "statement.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	class Scope : public Statement
	{
	public:
		class ExistsException : public std::exception
		{
		public:
			ExistsException(tree::Identity *_identity, tree::Identity *_conflictingIdentity) : identity(_identity), conflictingIdentity(_conflictingIdentity) {}

			tree::Identity *identity;
			tree::Identity *conflictingIdentity;
		};

		class NotFoundException : public std::exception
		{
		public:
			NotFoundException(tree::Identifier *_identifier) : identifier(_identifier) {}

			tree::Identifier *identifier;
		};

		Scope *getParent() const
		{
			return mParent;
		}

		void setParent(Scope *parent)
		{
			mParent = parent;
		}

		Identities &getIdentities()
		{
			return mIdentities;
		}

		Statements *getStatements() const
		{
			return mStatements;
		}

		virtual void checkIdentity(Identity *identity);
		void mapIdentity(Identity *identity);
		virtual Identity *findIdentity(Identifier *identifier);

	protected:
		Scope(Statements *statements) : mParent(NULL), mStatements(statements) { /* Abstract class */ }

		Scope *mParent;
		Identities mIdentities;

	private:
		Statements *mStatements;
	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class AnonymousScope : public Scope
	{
	public:
		AnonymousScope(Statements *statements) : Scope(statements) {}

#ifdef DEBUG
		virtual void printNode() { LOG("AnonymousScope"); }
#endif
	};

	class NamedScope : public Scope
	{
	public:
		NamedScope(std::string name, Statements *statements) : Scope(statements), mName(name) {}

		const std::string &getName() const
		{
			return mName;
		}

#ifdef DEBUG
		virtual void printNode() { LOG("NamedScope"); }
#endif

	private:
		std::string mName;
	};
}

#endif

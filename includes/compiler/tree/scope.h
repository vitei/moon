#ifndef COMPILER_TREE_SCOPE_H
#define COMPILER_TREE_SCOPE_H

#include <exception>
#include <map>
#include <string>
#include "node.h"
#include "statement.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

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

		Scope(Statements *statements) : mParent(NULL), mStatements(statements) {}

		Scope *getParent()
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

		Statements *getStatements()
		{
			return mStatements;
		}

		virtual void checkIdentity(Identity *identity);
		void mapIdentity(Identity *identity);
		virtual Identity *findIdentity(Identifier *identifier);

#ifdef DEBUG
		virtual void printNode() { LOG("Scope"); }
#endif

	protected:
		Scope *mParent;

	private:
		Identities mIdentities;
		Statements *mStatements;
	};
}

#endif

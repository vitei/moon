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

		std::map<std::string, Identity *> &getIdentities()
		{
			return mIdentities;
		}

		Statements *getStatements()
		{
			return mStatements;
		}

		void mapIdentity(Identity *identity);

	private:
		Scope *mParent;
		std::map<std::string, Identity *> mIdentities;
		Statements *mStatements;
	};
}

#endif

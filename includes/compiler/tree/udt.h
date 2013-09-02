#ifndef COMPILER_TREE_UDT_H
#define COMPILER_TREE_UDT_H

#include <string>
#include "expression.h"
#include "node.h"
#include "type.h"


namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

	class UDT : public Type
	{
	public:
		UDT(std::string name) : mName(name) {}

		const std::string &getName() const
		{
			return mName;
		}

		virtual const char *getTypeName() const
		{
			return mName.c_str();
		}

#ifdef DEBUG
		virtual void printNode() { LOG("UDT"); }
		virtual void printType() { LOG("UDT: %s", mName.c_str()); }
#endif

	protected:
		virtual bool equals(const Type &type) const;

	private:
		std::string mName;
	};
}

#endif

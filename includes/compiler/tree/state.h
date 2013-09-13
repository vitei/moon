#ifndef COMPILER_TREE_STATE_H
#define COMPILER_TREE_STATE_H

#include "common.h"

#include <string>
#include "node.h"

namespace tree
{
	/* ---- ONLY ABSTRACT CLASSES BELOW HERE ---- */

//	class State : public Node
//	{
//	protected:
//		State() { /* Abstract class */ }
//	};

	/* ---- ONLY CONCRETE CLASSES BELOW HERE ---- */

//	class DefaultState : public State
//	{
//	public:
//#ifdef DEBUG
//		virtual void printNode() { LOG("DefaultState"); }
//#endif
//	};

//	class NamedState : public State
//	{
//	public:
//		NamedState(const char *name) : mName(name) {}
//
//		const std::string &getName()
//		{
//			return mName;
//		}
//
//#ifdef DEBUG
//		virtual void printNode() { LOG("NamedState"); }
//#endif
//
//	private:
//		std::string mName;
//	};
}

#endif

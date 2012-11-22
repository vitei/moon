#ifndef TREE_PROGRAM_H
#define TREE_PROGRAM_H

#include "node.h"


namespace tree
{
	class Program : public Node
	{
	public:
		NodeList &getStatements()
		{
			return mStatements;
		}

	private:
		NodeList mStatements;
	};
}

#endif

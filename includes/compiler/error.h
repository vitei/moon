#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include "tree.h"


namespace error
{
	unsigned int count();
	void enqueue(const std::string &message);
	void enqueue(tree::Node::Location &location, const std::string &message);
	void enqueue(tree::Node::Location &conflictLocation, tree::Node::Location &location, const std::string &message);
	void output();
}

#endif

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include "tree.h"


namespace error
{
	unsigned int count();
	void enqueue(const std::string &message);
	void enqueue(const tree::Node::Location &location, const std::string &message);
	void enqueue(const tree::Node::Location &conflictLocation, const tree::Node::Location &location, const std::string &message);
	void output();
}

#endif

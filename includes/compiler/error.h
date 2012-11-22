#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

namespace error
{
	void enqueue(const char *message);
	void enqueue(unsigned int lineNumber, const char *message);
	void output();
}

#endif

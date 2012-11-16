#ifndef ERROR_H
#define ERROR_H

namespace error
{
	void enqueue(unsigned int lineNumber, const char *message);
	void output();
}

#endif

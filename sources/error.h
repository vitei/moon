#ifndef ERROR_H
#define ERROR_H

namespace error
{
	void enqueue(const char *message);
	void enqueue(unsigned int lineNumber, const char *message);
	void output();
}

#endif

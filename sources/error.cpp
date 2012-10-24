#include <iostream>
#include "error.h"


void emitError(const char *error)
{
	std::cerr << "Error: " << error << std::endl;
}

void emitError(unsigned int lineNum, const char *error)
{
	std::cerr << "Error on line #" << lineNum << ": " << error << std::endl;
}

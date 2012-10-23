#include <iostream>
#include "error.h"


void emitError(unsigned int lineNum, const char *error)
{
	std::cerr << "Error on line #" << lineNum << ": " << error << std::endl;
}

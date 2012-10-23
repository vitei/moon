#include <iostream>
#include "error.h"


void yyerror(const char *error)
{
	std::cerr << "Error: " << error << std::endl;
}

#include <iostream>
#include <stdio.h>
#include "tree.h"


// Bison and Flex stuff
extern int yyparse();
extern "C" FILE *yyin;

int main(int argc, char *argv[])
{
	for(unsigned int i = 1; i < argc; i++)
	{
		if((yyin = fopen(argv[i], "r")))
		{
			// Setup??
			tree::Block::setCurrentBlock(new tree::Block());

			yyparse();
			fclose(yyin);

			// ...
		}
		else
		{
			std::cerr << "Could not process file " << argv[i] << std::endl;
		}
	}

	return 0;
}

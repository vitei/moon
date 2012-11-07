#include <iostream>
#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include "tree.h"


extern void yyparse(void *scanner);

int main(int argc, char *argv[])
{
	for(unsigned int i = 1; i < argc; i++)
	{
		FILE *input;

		if((input = fopen(argv[i], "r")))
		{
			yyscan_t scanner;

			// Setup??
			tree::Block::setCurrentBlock(new tree::Block());
			yylex_init(&scanner);
			yyset_in(input, scanner);

			yyparse(scanner);

			yylex_destroy(scanner);
			fclose(input);

			// ...
		}
		else
		{
			std::cerr << "Could not process file " << argv[i] << std::endl;
		}
	}

	return 0;
}

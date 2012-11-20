#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>
#include "error.h"
#include "tree.h"

/* Generated headers */
#include "parser.h"
#include "lexer.h"

extern void yyparse(void *scanner);

int main(int argc, char *argv[])
{
	char opt;
	bool error = false;

	// Parse options
	opterr = 0;
	while((opt = getopt(argc, argv, "C:I:h")) != -1)
	{
		switch(opt)
		{
		case 'C':
			break;

		case 'I':
			break;

		case ':':
			std::cerr << "Option " << static_cast<char>(optopt) << " requires parameter" << std::endl;
			error = true;
			break;

		case '?':
			std::cerr << "Unknown option \"-" << static_cast<char>(optopt) << "\"" << std::endl;
			error = true;
			break;

		case 'h':
			error = true; // Not really an error but this will print usage...
			break;
		}
	}

	if(error)
	{
		std::cerr << "Usage: " << basename(argv[0]) << " [-C<directories>] [-I<directories>] [-h] <classes>" << std::endl
			<< "\t-C Scan directories for class files"<< std::endl
			<< "\t-I Scan directories for include files" << std::endl
			<< "\t-h Show this message" << std::endl;
	}
	else
	{
		for(; optind < argc; optind++)
		{
			FILE *input;

			if((input = fopen(argv[optind], "r")))
			{
				yyscan_t scanner;

				// Setup??
				//tree::Block::setCurrentBlock(new tree::Block());
				yylex_init(&scanner);
				yyset_in(input, scanner);

				yyparse(scanner);

				yylex_destroy(scanner);
				fclose(input);

				// ...


				// Show the errors
				error::output();
			}
			else
			{
				std::cerr << "Could not process file " << argv[optind] << std::endl;
			}
		}
	}

	return error ? 1 : 0;
}

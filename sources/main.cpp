#include <cstdio>
#include <libgen.h>
#include <unistd.h>
#include <iostream>
#include "error.h"
#include "loader.h"
#include "tree.h"

/* Generated headers */
#include "parser.h"
#include "lexer.h"


extern void yyparse(void *scanner);

const char *DIRECTORY_SEPARATORS = " ,:";

int main(int argc, char *argv[])
{
	char opt;
	bool error = false;

	// Parse options
	opterr = 0;
	while((opt = getopt(argc, argv, "U:I:h")) != -1)
	{
		switch(opt)
		{
		case 'U':
			for(char *directory = strtok(optarg, DIRECTORY_SEPARATORS); directory != 0; directory = strtok(0, DIRECTORY_SEPARATORS))
			{
				loader::addUseDirectory(directory);
			}

			break;

		case 'I':
			for(char *directory = strtok(optarg, DIRECTORY_SEPARATORS); directory != 0; directory = strtok(0, DIRECTORY_SEPARATORS))
			{
				loader::addIncludeDirectory(directory);
			}

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

	if(error) // I'm doing this backwards so that the usage text is near the command line option parser
	{
		std::cerr << "Usage: " << basename(argv[0]) << " [-C<directories>] [-I<directories>] [-h] <classes>" << std::endl
			<< "\t-U Scan directories for use files"<< std::endl
			<< "\t-I Scan directories for include files" << std::endl
			<< "\t-h Show this message" << std::endl;
	}
	else
	{
		char tmp[1024];

		// The current working directory must also be used...
		getcwd(tmp, 1024);
		loader::addUseDirectory(tmp);
		loader::addIncludeDirectory(tmp);

		for(; optind < argc; optind++)
		{
			FILE *input;

			if((input = loader::useFile(argv[optind])))
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
				std::cerr << "Could not load file " << argv[optind] << std::endl;
			}
		}
	}

	return error ? 1 : 0;
}

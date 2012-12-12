#include <cstdio>
#include <libgen.h>
#include <unistd.h>
#include <iostream>
#include "compiler/error.h"
#include "compiler/lexer.h"
#include "compiler/loader.h"
#include "compiler/operation.h"
#include "compiler/parser.h"
#include "compiler/tree.h"

/* Generated headers */
#include "generated_parser.h"
#include "generated_lexer.h"


extern int yyparse(parser::Data *parserData);

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
		loader::pushCWD(tmp);

		for(; optind < argc; optind++)
		{
			if(loader::useFile(argv[optind], tmp))
			{
				FILE *input = fopen(tmp, "r");
				parser::Data parserData;
				lexer::Data lexerData;

				lexerData.type = lexer::Data::TYPE_USE;
				lexerData.startSymbolIssued = false;

				// Setup??
				yylex_init_extra(&lexerData, &parserData.lexer);
				yyset_in(input, parserData.lexer);

				parserData.addParsedFile(tmp);
				loader::pushCWD(dirname(tmp));

				parserData.uses = new tree::Uses();

				yyparse(&parserData);

				loader::popCWD();

				yylex_destroy(parserData.lexer);
				fclose(input);

				// The program has been made...
				tree::Program program(parserData.uses);

				// We may now perform operations on it...
				//operation::MapIdentities mapIdentities(&program);
				//mapIdentities.run();
				//operation::resolveIdentifiers(program);

				// ...
			}
			else
			{
				std::string error = "Could not load file ";
				error += argv[optind];
				error::enqueue(error.c_str());
			}
		}

		// Show the errors
		error::output();
	}

	return error ? 1 : 0;
}

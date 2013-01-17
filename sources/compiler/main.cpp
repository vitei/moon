#include <cstring>
#include <libgen.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "compiler/error.h"
#include "compiler/generators.h"
#include "compiler/lexer.h"
#include "compiler/loader.h"
#include "compiler/operations.h"
#include "compiler/parser.h"
#include "compiler/tree.h"


const char *DIRECTORY_SEPARATORS = " ,:";

static parser::Data sParserData; // Debug filenames are maintained by this so it must persist throughout the program's life, FIXME?

static generator::C sCGenerator;
static generator::Generator *sGenerator;

int main(int argc, char *argv[])
{
	char opt;
	bool error = false;
	bool generateDefines = false;
	std::string outputFilename = "a.c";

	LOG("DEBUG OUTPUT IS ON!");

	sGenerator = &sCGenerator;

	// Parse options
	opterr = 0;
	while((opt = getopt(argc, argv, "D:dI:U:o:h")) != -1)
	{
		switch(opt)
		{
		case 'D':
			break;

		case 'd':
			generateDefines = true;

			break;

		case 'I':
			for(char *directory = strtok(optarg, DIRECTORY_SEPARATORS); directory != 0; directory = strtok(0, DIRECTORY_SEPARATORS))
			{
				loader::addIncludeDirectory(directory);
			}

			break;

		case 'U':
			for(char *directory = strtok(optarg, DIRECTORY_SEPARATORS); directory != 0; directory = strtok(0, DIRECTORY_SEPARATORS))
			{
				loader::addUseDirectory(directory);
			}

			break;

		case 'o':
			outputFilename = optarg;

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
		std::cerr << "Usage:" << std::endl
			<< "\t" << basename(argv[0]) << " [-D<dirs>] [-d] [-I<dirs>] [-U<dirs>] [-o<output>] [-h] <input>" << std::endl << std::endl
			<< "Options:" << std::endl
			<< "\t-D Scan directories for define files" << std::endl
			<< "\t-d Generate define file(s) for each input file" << std::endl
			<< "\t-I Scan directories for include files" << std::endl
			<< "\t-U Scan directories for use files"<< std::endl
			<< "\t-o Output location"<< std::endl
			<< "\t-h Show this message" << std::endl << std::endl
			<< "Notes:" << std::endl
			<< "\tDirectories may be separated by spaces, commas or colons." << std::endl;
	}
	else
	{
		if(optind == argc)
		{
			error::enqueue("No input files");
		}
		else
		{
			char tmp[1024];
			tree::Statements aggregates;
			std::string name;

			// The current working directory must also be used...
			getcwd(tmp, 1024);
			loader::pushCWD(tmp);

			for(; optind < argc; optind++)
			{
				if(loader::useFile(argv[optind], tmp))
				{
					std::string filename = tmp;

					loader::filenameToUseName(tmp, tmp);
					name = tmp;

					sParserData.uses = new tree::Statements();

					loader::pushCWD(dirname(tmp));
					sParserData.parseUse(name, filename);
					loader::popCWD();

					if(error::count() == 0)
					{
						aggregates.push_back(new tree::Aggregate(sParserData.uses));
					}
				}
				else
				{
					std::string error = "Could not load file ";
					error += argv[optind];
					error::enqueue(error.c_str());
				}
			}

			if(error::count() == 0)
			{
				// The program has been made...
				tree::Program program(name, &aggregates);

				// We may now perform operations on it...
				operation::ScopeParents::run(&program);
				operation::MapIdentities::run(&program);
				operation::ResolveIdentities::run(&program);

				if(generateDefines)
				{
					// If there are no errors we should be able to generate the defines
					if(error::count() == 0)
					{
						// ...
					}
				}
				else
				{
					operation::TypeExpressions::run(&program);

					// If there are no errors we should be able to do code generation now!
					if(error::count() == 0)
					{
						std::ofstream outputFile;

						outputFile.open(outputFilename.c_str());
						sGenerator->run(outputFile, &program);
						outputFile.close();
					}
				}
			}
		}

		// Show the errors
		error::output();
	}

	return error || error::count() != 0 ? 1 : 0;
}

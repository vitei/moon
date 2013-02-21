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

enum Generators
{
	GENERATOR_C,

	GENERATOR_MAX
};

static parser::Data sParserData; // Debug filenames are maintained by this so it must persist throughout the program's life, FIXME?

static generator::C sCGenerator;
static generator::Generator *sGenerator;
static generator::Generator *sGenerators[GENERATOR_MAX];

int main(int argc, char *argv[])
{
	char opt;
	bool usage = false;
	bool generateDefines = false;
	std::string outputFilename("a.c");

	LOG("DEBUG OUTPUT IS ON!");

	sGenerators[GENERATOR_C] = &sCGenerator;
	sGenerator = sGenerators[0];

	std::string baseOptions("D:dG:I:U:o:h");
	std::string options = baseOptions + sGenerator->getOptions();

	// Parse options
	opterr = 0;
	while((opt = getopt(argc, argv, options.c_str())) != -1)
	{
		switch(opt)
		{
		case 'D':
			break;

		case 'd':
			generateDefines = true;

			break;

		case 'G':
		{
			unsigned int generatorIdx = 0;

			for(; generatorIdx < GENERATOR_MAX; generatorIdx++)
			{
				if(sGenerators[generatorIdx]->getName().compare(optarg) == 0)
				{
					sGenerator = sGenerators[generatorIdx];
					break;
				}
			}

			if(generatorIdx == GENERATOR_MAX)
			{
				std::string error("Generator \"");
				error += optarg;
				error += "\" was not found";

				error::enqueue(error);
			}

			break;
		}

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
		{
			std::string error("Option \"");
			error += optopt;
			error += "\" requires parameter";

			error::enqueue(error);

			break;
		}

		case '?':
		{
			std::string error("Unknown option \"");
			error += optopt;
			error += "\" was passed";

			error::enqueue(error);

			break;
		}

		case 'h':
			usage = true;
			break;

		default:
			sGenerator->handleOption(opt, optarg, optopt);
			break;
		}

		options = baseOptions + sGenerator->getOptions();
	}

	if(usage) // I'm doing this backwards so that the usage text is near the command line option parser
	{
		std::cerr << "Usage:" << std::endl
			<< "\t" << basename(argv[0]) << " [-D<dirs>] [-d] [-G<generator>] [-I<dirs>] [-U<dirs>] [-o<output>] [-h]";

		if(sGenerator->optionsString() != "")
		{
			std::cerr << " " << sGenerator->optionsString();
		}

		std::cerr << " <input>" << std::endl << std::endl
			<< "Options:" << std::endl
			<< "\t-D Scan directories for define files" << std::endl
			<< "\t-d Generate define file(s) for each input file" << std::endl
			<< "\t-G Use the specified generator to generate output file" << std::endl
			<< "\t-I Scan directories for include files" << std::endl
			<< "\t-U Scan directories for use files"<< std::endl
			<< "\t-o Output location"<< std::endl
			<< "\t-h Show this message" << std::endl << std::endl
			<< "Generator options:" << std::endl
			<< sGenerator->optionsHelpString() << std::endl << std::endl;

		std::cerr << "Avaliable Generators:" << std::endl
			<< "\t" << sGenerators[0]->getName() << " (default)" << std::endl;

		for(unsigned int i = 1; i < GENERATOR_MAX; i++)
		{
			std::cerr << "\t" << sGenerators[i]->getName() << std::endl;
		}


		std::cerr << std::endl << "Notes:" << std::endl
			<< "\tDirectories may be separated by spaces, commas or colons." << std::endl << std::endl;
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

					loader::pushCWD(dirname(const_cast<char *>(filename.c_str())));
					sParserData.parseUse(name, filename);
					loader::popCWD();

					if(error::count() == 0)
					{
						aggregates.push_back(new tree::Aggregate(sParserData.uses));
					}
				}
				else
				{
					std::string error("Could not load file ");
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

	return usage ? 2 : (error::count() != 0 ? 1 : 0);
}

#include "common.h"

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

typedef void (*TreeOperation)(tree::Program *program);
typedef bool (*ValidatingTreeOperation)(tree::Program *program);

enum Generators
{
	GENERATOR_C,
	GENERATOR_OBJ_C,

	GENERATOR_MAX
};

static parser::Data sParserData; // Debug filenames are maintained by this so it must persist throughout the program's life, FIXME?

static generator::c::Generator sCGenerator;
static generator::objC::Generator sObjCGenerator;
static generator::Generator *sGenerator;
static generator::Generator *sGenerators[GENERATOR_MAX];

void doOperations(tree::Program &program, TreeOperation *operations)
{
	for(unsigned char i = 0; operations[i]; i++)
	{
		operations[i](&program);

		if(error::count() != 0)
		{
			LOG("Errors encountered in stage %u", i);
			break;
		}
	}
}

bool doValidatingOperations(tree::Program &program, ValidatingTreeOperation *operations)
{
	bool r = true;

	for(unsigned char i = 0; operations[i]; i++)
	{
#ifdef DEBUG
		bool rerunUnneeded = operations[i](&program);

		if(!rerunUnneeded)
		{
			LOG("Re-run scheduled from stage %u", i);
		}

		r &= rerunUnneeded;
#else
		r &= operations[i](&program);
#endif

		if(error::count() != 0)
		{
			LOG("Errors encountered in stage %u", i);
			break;
		}
	}

	return r;
}

void resolveProgram(tree::Program *program)
{
	// The list of operations to perform...
	ValidatingTreeOperation operations[] = {
		// Access to UDT members requires type resolution
		operation::ResolveIdentities::run,

		// First we need to compute any constants we can so that types that use these as sizes
		operation::ComputeConstants::run,

		// Next we can resolve and infer any types
		// Types may use constants for their inference...
		operation::ResolveTypes::run,
		operation::InferTypes::run,

		// Now we can create any castings that are needed for the next pass...
		operation::CastExpressions::run,

		// Check that these typecasts are OK
		operation::CheckTypecasting::run,

		nullptr
	};

	for(bool validated = false; !validated && !error::count(); validated = doValidatingOperations(*program, operations))
		;
}


int main(int argc, char *argv[])
{
	char opt;
	bool usage = false;
	bool generateDefines = false;

	LOG("DEBUG OUTPUT IS ON!");

	sGenerators[GENERATOR_C] = &sCGenerator;
	sGenerators[GENERATOR_OBJ_C] = &sObjCGenerator;
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
		std::cerr << "Usage:\n"
		             "\t"
		          << basename(argv[0])
		          << " [-D<dirs>] [-d] [-G<generator>] [-I<dirs>] [-U<dirs>] [-h]";

		if(sGenerator->optionsString() != "")
		{
			std::cerr << " " << sGenerator->optionsString();
		}

		std::cerr << " <input>\n\n"

		             "Options:\n"
		             "\t-D Scan directories for define files\n"
		             "\t-d Generate define file(s) for each input file\n"
		             "\t-G Use the specified generator to generate output file\n"
		             "\t-I Scan directories for include files\n"
		             "\t-U Scan directories for use files\n"
		             "\t-h Show this message\n\n"

		             "Generator options:\n"
		          << sGenerator->optionsHelpString()
		          << "\n\n";

		std::cerr << "Avaliable Generators:\n"
		             "\t"
		          << sGenerators[0]->getName()
		          << " (default)\n";

		for(unsigned int i = 1; i < GENERATOR_MAX; i++)
		{
			std::cerr << "\t" << sGenerators[i]->getName() << "\n";
		}

		std::cerr << "\n"
		             "Notes:\n"
		             "\tDirectories may be separated by spaces, commas or colons.\n";
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

					loader::pushCWD(dirname(tmp));

					loader::filenameToUseName(tmp, filename.c_str());
					name = tmp;

					sParserData.uses = new tree::Statements();

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

				// The list of operations to perform...
				TreeOperation operations[] = {
					operation::ScopeParents::run,
					operation::CheckScoping::run,

					resolveProgram,

					// Remove any junk nodes
					operation::RemoveDeadNodes::run,

					// FIXME, need an operation here to get all variable definition statements and assign suitable default values.

					nullptr
				};

				doOperations(program, operations);

				// If there are no errors we should be able to do code generation now
				if(error::count() == 0)
				{
					if(generateDefines)
					{
						// ...
					}
					else
					{
						sGenerator->run(&program);
					}
				}
			}
		}

		// Show the errors
		error::output();
	}

	return usage ? 2 : (error::count() != 0 ? 1 : 0);
}

#include "common.h"

#include <fstream>
#include "compiler/tree.h"
#include "compiler/generators/c/generator.h"
#include "compiler/generators/c/mangle_names.h"
#include "compiler/generators/c/output_types.h"
#include "compiler/generators/c/output_variables.h"
#include "compiler/generators/c/output_function_prototypes.h"
#include "compiler/generators/c/output_functions.h"
#include "compiler/generators/c/output_new.h"

void generator::c::Generator::run(tree::Program *program)
{
	std::ofstream outputFile;

	outputFile.open(mOutputFilename.c_str());

	mPrinter.init(outputFile, program);
	generate(program);

	outputFile.close();
}

std::string generator::c::Generator::getOptions()
{
	return "Bo";
}

void generator::c::Generator::handleOption(char opt, char *optarg, int optopt)
{
	switch(opt)
	{
		case 'B':
			mIsBoostrapped = true;
			break;

		case 'o':
			mOutputFilename = optarg;

			break;
	}
}

std::string generator::c::Generator::optionsString()
{
	return "[-B] [-o<output>]";
}

std::string generator::c::Generator::optionsHelpString()
{
	return "\t-B Output bootstrap code\n"
	       "\t-o Output C code path";

}

void generator::c::Generator::generate(tree::Program *program)
{
	mangleNames(program);

	mPrinter.outputTabs();
	mPrinter.outputPragma("#include <mrl.h>");

	outputTypes(program);
	outputVariables(program);
	outputFunctions(program);
	outputNew(program);

	if(mIsBoostrapped)
	{
		mPrinter.outputBootstrapMain();
	}
}

void generator::c::Generator::mangleNames(tree::Program *program)
{
	generator::c::MangleNames::run(program);
}

void generator::c::Generator::outputTypes(tree::Program *program)
{
	generator::c::OutputTypes::run(&mPrinter, program);
}

void generator::c::Generator::outputVariables(tree::Program *program)
{
	generator::c::OutputVariables::run(&mPrinter, program);
}

void generator::c::Generator::outputFunctions(tree::Program *program)
{
	generator::c::OutputFunctionPrototypes::run(&mPrinter, program);
	generator::c::OutputFunctions::run(&mPrinter, program);
}

void generator::c::Generator::outputNew(tree::Program *program)
{
	generator::c::OutputNew::run(&mPrinter, program);
}


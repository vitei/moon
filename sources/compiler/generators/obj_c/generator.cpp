#include "common.h"

#include <fstream>
#include "compiler/tree.h"
#include "compiler/generators/obj_c/generator.h"
#include "compiler/generators/obj_c/mangle_names.h"
#include "compiler/generators/obj_c/output_types.h"
#include "compiler/generators/obj_c/output_variables.h"
#include "compiler/generators/obj_c/output_function_prototypes.h"
#include "compiler/generators/obj_c/output_functions.h"
#include "compiler/generators/obj_c/output_new.h"

void generator::objC::Generator::run(tree::Program *program)
{
	std::ofstream outputFile;

	outputFile.open(mOutputFilename.c_str());

	mPrinter.init(outputFile, program);
	generate(program);

	outputFile.close();
}

std::string generator::objC::Generator::getOptions()
{
	return "Bo";
}

void generator::objC::Generator::handleOption(char opt, char *optarg, int optopt)
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

std::string generator::objC::Generator::optionsString()
{
	return "[-B] [-o<output>]";
}

std::string generator::objC::Generator::optionsHelpString()
{
	return "\t-B Output bootstrap code\n"
	       "\t-o Output C code path";

}

void generator::objC::Generator::generate(tree::Program *program)
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

void generator::objC::Generator::mangleNames(tree::Program *program)
{
	generator::objC::MangleNames::run(program);
}

void generator::objC::Generator::outputTypes(tree::Program *program)
{
	generator::objC::OutputTypes::run(&mPrinter, program);
}

void generator::objC::Generator::outputVariables(tree::Program *program)
{
	generator::objC::OutputVariables::run(&mPrinter, program);
}

void generator::objC::Generator::outputFunctions(tree::Program *program)
{
	generator::objC::OutputFunctionPrototypes::run(&mPrinter, program);
	generator::objC::OutputFunctions::run(&mPrinter, program);
}

void generator::objC::Generator::outputNew(tree::Program *program)
{
	generator::objC::OutputNew::run(&mPrinter, program);
}


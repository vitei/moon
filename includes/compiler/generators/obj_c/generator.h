#ifndef COMPILER_GENERATORS_OBJ_C_GENERATOR_H
#define COMPILER_GENERATORS_OBJ_C_GENERATOR_H

#include "common.h"

#include <string>
#include "compiler/tree.h"
#include "compiler/generators/generator.h"
#include "printer.h"

namespace generator
{
	namespace objC
	{
		class Generator final : public generator::Generator
		{
		public:
			Generator() : mIsBoostrapped(false), mOutputFilename("a.m") {}

			virtual void run(tree::Program *program);

			virtual std::string getName()
			{
				return "Objective-C";
			}

			virtual std::string getOptions();
			virtual void handleOption(char opt, char *optarg, int optopt);
			virtual std::string optionsString();
			virtual std::string optionsHelpString();

		private:
			virtual void generate(tree::Program *program);

			void mangleNames(tree::Program *program);
			void outputTypes(tree::Program *program);
			void outputVariables(tree::Program *program);
			void outputFunctions(tree::Program *program);
			void outputNew(tree::Program *program);

			generator::objC::Printer mPrinter;
			bool mIsBoostrapped;
			std::string mOutputFilename;
		};
	}
}

#endif

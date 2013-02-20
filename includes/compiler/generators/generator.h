#ifndef COMPILER_GENERATORS_GENERATOR_H
#define COMPILER_GENERATORS_GENERATOR_H

#include <ostream>
#include <string>
#include "compiler/tree.h"
#include "compiler/operations/operation.h"


namespace generator
{
	class Generator
	{
	public:
		virtual void run(std::ostream &output, tree::Program *program) = 0;

		virtual std::string getName() = 0;

		virtual std::string getOptions()
		{
			return "";
		}

		virtual void handleOption(char opt, char *optarg, int optopt)
		{
		}

		virtual std::string optionsHelpString()
		{
			return "\tNo options";
		}

	protected:
		Generator() { /* Abstract class */ }
	};
}

#endif

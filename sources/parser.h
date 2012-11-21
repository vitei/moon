#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "tree/program.h"


namespace parser
{
	class Data
	{
	public:
		void addParsedFile(const char *filename)
		{
			mParsedFiles.push_back(std::string(filename));
		}

		bool isParsedFile(const char *filename)
		{
			for(std::vector<std::string>::iterator i = mParsedFiles.begin(), end = mParsedFiles.end(); i != end; ++i)
			{
				if(*i == filename)
				{
					return true;
				}
			}

			return false;
		}

		void *lexer;

		tree::Program &getProgram()
		{
			return mProgram;
		}

	private:
		std::vector<std::string> mParsedFiles;
		tree::Program mProgram;
	};
}

#endif

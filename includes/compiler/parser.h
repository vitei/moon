#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <string>
#include <vector>
#include "tree.h"


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

		// FIXME, these three need to be nicer...
		void *lexer;
		tree::Statement *includeHead;
		tree::Statement *includeTail;
		tree::Scope *scope;

	private:
		std::vector<std::string> mParsedFiles;
	};
}

#endif

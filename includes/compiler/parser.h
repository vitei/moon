#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <string>
#include <vector>
#include "tree.h"


namespace parser
{
	class StatementList
	{
	public:
		tree::Statement *head;
		tree::Statement *tail;
	};

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
		StatementList list;

	private:
		std::vector<std::string> mParsedFiles;
	};
}

#endif

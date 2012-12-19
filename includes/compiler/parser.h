#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <string>
#include <vector>
#include "lexer.h"
#include "tree.h"


namespace parser
{
	class Data
	{
	public:
		void addParsedFile(const std::string &filename)
		{
			mParsedFiles.push_back(filename);
		}

		bool isParsedFile(const std::string &filename)
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

		void parse(lexer::Data::FileType type, std::string filename);

		// FIXME, these three need to be nicer...
		std::string *currentFilename;
		void *lexer;
		tree::Statements *uses;
		tree::Statements *statements;

	private:
		std::vector<std::string> mParsedFiles;
	};
}

#define YYLTYPE tree::Node::Location

#endif

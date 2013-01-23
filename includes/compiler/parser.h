#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <map>
#include <string>
#include <vector>
#include "lexer.h"
#include "tree.h"


namespace parser
{
	class Data
	{
	public:
		void addParsedUseFile(const std::string &filename)
		{
			mParsedUseFiles.push_back(filename);
		}

		void addParsedIncludeFile(const std::string &filename)
		{
			mParsedIncludeFiles[currentName].push_back(filename);
		}

		bool isParsedUseFile(const std::string &filename)
		{
			for(std::vector<std::string>::iterator i = mParsedUseFiles.begin(), end = mParsedUseFiles.end(); i != end; ++i)
			{
				if(*i == filename)
				{
					return true;
				}
			}

			return false;
		}

		bool isParsedIncludeFile(const std::string &filename)
		{
			std::vector<std::string> currentIncludeFiles = mParsedIncludeFiles[currentName];

			for(std::vector<std::string>::iterator i = currentIncludeFiles.begin(), end = currentIncludeFiles.end(); i != end; ++i)
			{
				if(*i == filename)
				{
					return true;
				}
			}

			return false;
		}

		void parseUse(const std::string &name, const std::string &filename);
		void parseInclude(const std::string &filename);

		// FIXME, these three need to be nicer...
		std::string currentName;
		std::string *currentFilename;
		void *lexer;
		tree::Statements *uses;
		tree::Statements *statements;

	private:
		void parse(lexer::Data::FileType type, const std::string &filename);

		std::vector<std::string> mParsedUseFiles;
		std::map<std::string, std::vector<std::string> > mParsedIncludeFiles;
	};
}

#define YYLTYPE tree::Node::Location

#endif

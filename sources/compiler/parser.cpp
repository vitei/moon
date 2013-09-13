#include "common.h"

#include <cstdio>
#include "compiler/lexer.h"
#include "compiler/parser.h"

/* Generated headers */
#include "generated/parser.h"
#include "generated/lexer.h"

extern int yyparse(parser::Data *parserData);

void parser::Data::parse(lexer::Data::FileType type, std::string *filename)
{
	lexer::Data lexerData;
	void *previousLexer = lexer;
	std::string *previousFilename = currentFilename;
	FILE *input = fopen(filename->c_str(), "r");

	currentFilename = filename;

	lexerData.type = type;
	lexerData.startSymbolIssued = false;

	yylex_init_extra(&lexerData, &lexer);
	yyset_in(input, lexer);
	yyparse(this);
	yylex_destroy(lexer);
	fclose(input);

	lexer = previousLexer;
	currentFilename = previousFilename;
}

void parser::Data::parseUse(const std::string &name, const std::string &filename)
{
	std::string *filenamePtr = new std::string(filename);
	std::string previousName = currentName;

	mParsedUseFiles.push_back(filenamePtr);

	currentName = name;

	if(filename.rfind(".lmoon") == filename.length() - 6)
	{
		parse(lexer::Data::TYPE_LITERATE_USE, filenamePtr);
	}
	else
	{
		parse(lexer::Data::TYPE_USE, filenamePtr);
	}

	currentName = previousName;
}

void parser::Data::parseInclude(const std::string &filename)
{
	std::string *filenamePtr = new std::string(filename);

	mParsedIncludeFiles[currentName].push_back(filenamePtr);

	parse(lexer::Data::TYPE_INCLUDE, filenamePtr);
}

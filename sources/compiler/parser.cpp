#include <cstdio>
#include "compiler/lexer.h"
#include "compiler/parser.h"

/* Generated headers */
#include "generated/parser.h"
#include "generated/lexer.h"


extern int yyparse(parser::Data *parserData);

void parser::Data::parse(lexer::Data::FileType type, const std::string &filename)
{
	lexer::Data lexerData;
	void *tmp = lexer;
	FILE *input = fopen(filename.c_str(), "r");

	currentFilename = &const_cast<std::string &>(filename);

	lexerData.type = type;
	lexerData.startSymbolIssued = false;

	yylex_init_extra(&lexerData, &lexer);
	yyset_in(input, lexer);
	yyparse(this);
	yylex_destroy(lexer);
	fclose(input);

	lexer = tmp;
}

void parser::Data::parseUse(const std::string &name, const std::string &filename)
{
	std::string previousName = currentName;

	currentName = name;

	addParsedUseFile(filename);

	parse(lexer::Data::TYPE_USE, filename);
	currentName = previousName;
}

void parser::Data::parseInclude(const std::string &filename)
{
	addParsedIncludeFile(filename);
	parse(lexer::Data::TYPE_INCLUDE, filename);
}

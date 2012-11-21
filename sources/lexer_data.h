#ifndef LEXER_DATA_H
#define LEXER_DATA_H

class LexerData
{
public:
	enum FileType
	{
		TYPE_USE,
		TYPE_INCLUDE
	};

	FileType type;
	bool startSymbolIssued;
};

#endif

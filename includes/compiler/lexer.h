#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

namespace lexer
{
	class Data
	{
	public:
		enum FileType
		{
			TYPE_USE,
			TYPE_LITERAL_USE,
			TYPE_INCLUDE
		};

		FileType type;
		bool startSymbolIssued;
	};
}

#endif

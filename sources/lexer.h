#ifndef LEXER_H
#define LEXER_H

namespace lexer
{
	class Data
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
}

#endif

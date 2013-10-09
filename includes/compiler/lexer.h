#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

namespace lexer
{
	class Data final
	{
	public:
		enum FileType
		{
			TYPE_USE,
			TYPE_LITERATE_USE,
			TYPE_INCLUDE
		};

		FileType type;
		bool startSymbolIssued;
	};
}

#endif

#ifndef TREE_IDENTIFIER_H
#define TREE_IDENTIFIER_H

#include <string>


namespace tree
{
	class Identifier
	{
	public:
		enum Type
		{
			TYPE_VARIABLE,
			TYPE_REFERENCE
		};

		Identifier(Type type) : mType(type) {}

		std::string &getName()
		{
			return mName;
		}

		Type getIdentifierType()
		{
			return mType;
		}

	private:
		std::string mName;
		Type mType;
	};
}

#endif

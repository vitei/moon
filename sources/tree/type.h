#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include <string>


namespace tree
{
	class Type
	{
	public:
		enum Data
		{
			DATA_INT,
			DATA_FLOAT,
			DATA_STRING,
			DATA_UDT
		};

		Type(Data dataType) : mDataType(dataType) {}

	private:
		Data mDataType;
	};

	class UDTType : public Type
	{
	public:
		UDTType(const char *udtName) : Type(Type::DATA_UDT), mUDTName(udtName) {}

	private:
		std::string mUDTName;
	};
}

#endif

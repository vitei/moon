#include <iostream>
#include <vector>
#include "error.h"


class ErrorMessage
{
public:
	unsigned int mLineNumber;
	std::string mMessage;
};

static std::vector<ErrorMessage> sErrorList;

//void emitError(const char *error)
//{
//	std::cerr << "Error: " << error << std::endl;
//}

void error::enqueue(unsigned int lineNumber, const char *message)
{
	ErrorMessage errorMessage;

	errorMessage.mLineNumber = lineNumber;
	errorMessage.mMessage = std::string(message);

	sErrorList.push_back(errorMessage);
}

void error::output()
{
	for(std::vector<ErrorMessage>::iterator i = sErrorList.begin(), end = sErrorList.end(); i != end; ++i)
	{
		std::cerr << "Error on line #" << i->mLineNumber << ": " << i->mMessage << std::endl;
	}
}

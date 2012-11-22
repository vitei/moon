#include <iostream>
#include <vector>
#include "error.h"


class Error
{
public:
	Error(const char *message) : mMessage(std::string(message)) {}

	virtual void output()
	{
		std::cerr << "\033[1;31mError\033[0m: " << mMessage << std::endl;
	}

protected:
	std::string mMessage;
};

class FileError : public Error
{
public:
	FileError(const char *filename, const char *message) : Error(message), mFilename(std::string(filename)) {}

	virtual void output()
	{
		Error::output();
		std::cerr << "\tFile: " << mFilename << std::endl;
	}

protected:
	std::string mFilename;
};

class SyntaxError : public FileError
{
public:
	SyntaxError(unsigned int lineNumber, const char *filename, const char *message) : FileError(filename, message), mLineNumber(lineNumber) {}

	virtual void output()
	{
		FileError::output();
		std::cerr << "\tLine #" << mLineNumber << std::endl;
	}

protected:
	unsigned int mLineNumber;
};

static std::vector<Error *> sErrorList;

void error::enqueue(const char *message)
{
	sErrorList.push_back(new Error(message));
}

void error::enqueue(unsigned int lineNumber, const char *message)
{
	sErrorList.push_back(new SyntaxError(lineNumber, "", message));
}

void error::output()
{
	for(std::vector<Error *>::iterator i = sErrorList.begin(), end = sErrorList.end(); i != end; ++i)
	{
		(*i)->output();
	}
}

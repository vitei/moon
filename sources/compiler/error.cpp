#include <iostream>
#include <vector>
#include "compiler/error.h"


class Error
{
public:
	Error(const std::string &message) : mMessage(message) {}

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
	FileError(const tree::Node::Location &location, const std::string &message) : Error(message), mFilename(*location.filename) {}

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
	SyntaxError(const tree::Node::Location &location, const std::string &message) : FileError(location, message), mLineNumber(location.start.line) {}

	virtual void output()
	{
		FileError::output();
		std::cerr << "\tLine #" << mLineNumber << std::endl;
	}

protected:
	unsigned int mLineNumber;
};

static std::vector<Error *> sErrorList;

unsigned int error::count()
{
	return sErrorList.size();
}

void error::enqueue(const std::string &message)
{
	sErrorList.push_back(new Error(message));
}

void error::enqueue(tree::Node::Location &location, const std::string &message)
{
	sErrorList.push_back(new SyntaxError(location, message));
}

void error::output()
{
	for(std::vector<Error *>::iterator i = sErrorList.begin(), end = sErrorList.end(); i != end; ++i)
	{
		(*i)->output();
	}
}

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
	FileError(const tree::Node::Location &location, const std::string &message) : Error(message), mLocation(location) {}

	virtual void output()
	{
		Error::output();
		std::cerr << "\tFile: " << *mLocation.filename << std::endl;
	}

protected:
	tree::Node::Location mLocation;
};

class SyntaxError : public FileError
{
public:
	SyntaxError(const tree::Node::Location &location, const std::string &message) : FileError(location, message) {}

	virtual void output()
	{
		FileError::output();
		std::cerr << "\tLine: #" << mLocation.start.line << std::endl;
	}
};

class ConflictError : public SyntaxError
{
public:
	ConflictError(const tree::Node::Location &conflictLocation, const tree::Node::Location &location, const std::string &message) : SyntaxError(location, message), mConflictLocation(conflictLocation) {}

	virtual void output()
	{
		SyntaxError::output();

		ASSERT(mConflictLocation.filename);
		ASSERT(mLocation.filename);

		if(*mConflictLocation.filename != *mLocation.filename)
		{
			std::cerr << "\tConflicting File: " << *mConflictLocation.filename << std::endl;
		}

		std::cerr << "\tConflicting Line: #" << mConflictLocation.start.line << std::endl;
	}

protected:
	tree::Node::Location mConflictLocation;
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

void error::enqueue(tree::Node::Location &conflictLocation, tree::Node::Location &location, const std::string &message)
{
	sErrorList.push_back(new ConflictError(conflictLocation, location, message));
}

void error::output()
{
	for(std::vector<Error *>::iterator i = sErrorList.begin(), end = sErrorList.end(); i != end; ++i)
	{
		(*i)->output();
	}
}

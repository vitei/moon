#include <cctype>
#include <cstring>
#include <stack>
#include <string>
#include <vector>
#include <unistd.h>
#include "compiler/loader.h"


static std::vector<std::string> sUseDirectories;
static std::vector<std::string> sIncludeDirectories;
static std::stack<std::string> sCurrentWorkingDirectory;

void loader::addUseDirectory(const char *directory)
{
	sUseDirectories.push_back(std::string(directory));
}

void loader::addIncludeDirectory(const char *directory)
{
	sIncludeDirectories.push_back(std::string(directory));
}

void loader::pushCWD(const char *directory)
{
	sCurrentWorkingDirectory.push(std::string(directory));
}

void loader::popCWD()
{
	sCurrentWorkingDirectory.pop();
}

void loader::filenameToUseName(char *name, const char *filename)
{
	char *nameCurrent = name;

	// First character is always upper case
	for(bool nextUpper = true; *filename != '.'; filename++)
	{
		if(*filename == '_')
		{
			nextUpper = true;
		}
		else if(*filename == '/')
		{
			nextUpper = true;
			nameCurrent = name;
		}
		else if(nextUpper)
		{
			*nameCurrent++ = toupper(*filename);
			nextUpper = false;
		}
		else
		{
			*nameCurrent++ = *filename;
		}
	}

	*nameCurrent = NULL;

	ASSERT(strcmp(filename, ".moon") == 0);
}

void loader::filenameToIncludeName(char *name, const char *filename)
{
	char *nameCurrent = name;

	for(; *filename != '.'; filename++)
	{
		if(*filename == '/')
		{
			nameCurrent = name;
		}
		else
		{
			*nameCurrent++ = *filename;
		}
	}

	*nameCurrent = NULL;

	ASSERT(strcmp(filename, ".minc") == 0);
}

void loader::useNameToFilename(char *filename, const char *name)
{
	// First character is always upper case
	*filename++ = tolower(*name++);

	for(; *name; name++)
	{
		if(isupper(*name))
		{
			*filename++ = '_';
			*filename++ = tolower(*name);
		}
		else
		{
			*filename++ = *name;
		}
	}

	strcpy(filename, ".moon");
}

void loader::includeNameToFilename(char *filename, const char *name)
{
	for(; *name; *filename++ = *name++);
	strcpy(filename, ".minc");
}

bool loader::useFile(const char *filename, char *usedFilename)
{
	bool r;

	if(*filename == '/')
	{
		r = access(filename, R_OK) == 0;

		if(r && usedFilename)
		{
			strcpy(usedFilename, filename);
		}
	}
	else
	{
		std::string resolvedFilename = sCurrentWorkingDirectory.top() + "/" + filename;

		r = access(resolvedFilename.c_str(), R_OK) == 0;

		for(std::vector<std::string>::iterator i = sUseDirectories.begin(), end = sUseDirectories.end(); !r && i != end; ++i)
		{
			resolvedFilename = *i + "/" + filename;
			r = access(resolvedFilename.c_str(), R_OK) == 0;
		}

		if(r && usedFilename)
		{
			strcpy(usedFilename, resolvedFilename.c_str());
		}
	}

	return r;
}

bool loader::includeFile(const char *filename, char *usedFilename)
{
	std::string resolvedFilename = sCurrentWorkingDirectory.top() + "/" + filename;
	bool r = access(resolvedFilename.c_str(), R_OK) == 0;

	for(std::vector<std::string>::iterator i = sIncludeDirectories.begin(), end = sIncludeDirectories.end(); !r && i != end; ++i)
	{
		resolvedFilename = *i + "/" + filename;
		r = access(resolvedFilename.c_str(), R_OK) == 0;
	}

	if(r && usedFilename)
	{
		strcpy(usedFilename, resolvedFilename.c_str());
	}

	return r;
}

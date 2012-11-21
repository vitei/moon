#include <cctype>
#include <stack>
#include <string>
#include <vector>
#include "loader.h"


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

FILE *loader::useFile(const char *filename, char *usedFilename)
{
	std::string resolvedFilename = sCurrentWorkingDirectory.top() + "/" + filename;
	FILE *r = fopen(resolvedFilename.c_str(), "r");

	for(std::vector<std::string>::iterator i = sUseDirectories.begin(), end = sUseDirectories.end(); !r && i != end; ++i)
	{
		resolvedFilename = *i + "/" + filename;
		r = fopen(resolvedFilename.c_str(), "r");
	}

	if(r && usedFilename)
	{
		strcpy(usedFilename, resolvedFilename.c_str());
	}

	return r;
}

FILE *loader::includeFile(const char *filename, char *usedFilename)
{
	std::string resolvedFilename = sCurrentWorkingDirectory.top() + "/" + filename;
	FILE *r = fopen(resolvedFilename.c_str(), "r");

	for(std::vector<std::string>::iterator i = sIncludeDirectories.begin(), end = sIncludeDirectories.end(); !r && i != end; ++i)
	{
		resolvedFilename = *i + "/" + filename;
		r = fopen(resolvedFilename.c_str(), "r");
	}

	if(r && usedFilename)
	{
		strcpy(usedFilename, resolvedFilename.c_str());
	}

	return r;
}

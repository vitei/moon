#include <cctype>
#include <string>
#include <vector>
#include "loader.h"


static std::vector<std::string> sUseDirectories;
static std::vector<std::string> sIncludeDirectories;

void loader::addUseDirectory(const char *directory)
{
	sUseDirectories.push_back(std::string(directory));
}

void loader::addIncludeDirectory(const char *directory)
{
	sIncludeDirectories.push_back(std::string(directory));
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

FILE *loader::useFile(const char *filename)
{
	FILE *r = 0;

	for(std::vector<std::string>::iterator i = sUseDirectories.begin(), end = sUseDirectories.end(); !r && i != end; ++i)
	{
		std::string str = *i + "/" + filename;

		r = fopen(str.c_str(), "r");
	}

	return r;
}

FILE *loader::includeFile(const char *filename)
{
	FILE *r = 0;

	for(std::vector<std::string>::iterator i = sIncludeDirectories.begin(), end = sIncludeDirectories.end(); !r && i != end; ++i)
	{
		std::string str = *i + "/" + filename;

		r = fopen(str.c_str(), "r");
	}

	return r;
}

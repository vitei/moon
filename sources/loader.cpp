#include "loader.h"


void loader::addClassDirectory(const char *directory)
{

}

void loader::addIncludeDirectory(const char *directory)
{
	sIncludeDirectories.push_back(std::string(directory));
}

void loader::classNameToFilename(char *filename, const char *className)
{
	// First character is always upper case
	*filename++ = tolower(*className);

	for(const char *name = className + 1; *name; name++)
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

FILE *loader::classFile(const char *filename)
{
	FILE *r = 0;

	for(std::vector<std::string>::iterator i = sClassDirectories.begin(), end = sClassDirectories.end(); !r && i != end; ++i)
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

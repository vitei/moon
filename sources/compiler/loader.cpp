#include "common.h"

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

	*nameCurrent = 0;

	ASSERT(strcmp(filename, ".moon") == 0 || strcmp(filename, ".lmoon") == 0);
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

	*nameCurrent = 0;

	ASSERT(strcmp(filename, ".minc") == 0);
}

void loader::useNameToFilename(char *filename, const char *name)
{
	char *f = filename;
	// First character is always upper case
	*f++ = tolower(*name++);

	for(; *name; name++)
	{
		if(isupper(*name))
		{
			*f++ = '_';
			*f++ = tolower(*name);
		}
		else
		{
			*f++ = *name;
		}
	}

	// Check for literate moon file first...
	strcpy(f, ".lmoon");

	if(!loader::useFile(filename, nullptr))
	{
		strcpy(f, ".moon");
	}
}

void loader::includeNameToFilename(char *filename, const char *name)
{
	for(; *name; *filename++ = *name++);
	strcpy(filename, ".minc");
}

bool loader::useFile(const char *filename, char *usedFilename)
{
	bool r = false;

	if(isAbsolutePath(filename))
	{
		r = access(filename, R_OK) == 0;

		if(r && usedFilename)
		{
			strcpy(usedFilename, filename);
		}
	}

	if(!r)
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

bool loader::includeFile(const char *filename, char *includedFilename)
{
	bool r = false;

	if(isAbsolutePath(filename))
	{
		r = access(filename, R_OK) == 0;

		if(r && includedFilename)
		{
			strcpy(includedFilename, filename);
		}
	}

	if(!r)
	{
		std::string resolvedFilename = sCurrentWorkingDirectory.top() + "/" + filename;

		r = access(resolvedFilename.c_str(), R_OK) == 0;

		for(std::vector<std::string>::iterator i = sIncludeDirectories.begin(), end = sIncludeDirectories.end(); !r && i != end; ++i)
		{
			resolvedFilename = *i + "/" + filename;
			r = access(resolvedFilename.c_str(), R_OK) == 0;
		}

		if(r && includedFilename)
		{
			strcpy(includedFilename, resolvedFilename.c_str());
		}
	}

	return r;
}

bool loader::isAbsolutePath(const char *filename)
{
#ifdef _WIN32
	return filename[1] == ':';
#else
	return *filename == '/';
#endif
}

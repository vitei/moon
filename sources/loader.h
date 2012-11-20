#ifndef LOADER_H
#define LOADER_H

#include <cstdio>


namespace loader
{
	void addUseDirectory(const char *directory);
	void addIncludeDirectory(const char *directory);

	void useNameToFilename(char *filename, const char *name);

	FILE *useFile(const char *filename);
	FILE *includeFile(const char *filename);
}

#endif

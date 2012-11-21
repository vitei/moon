#ifndef LOADER_H
#define LOADER_H

#include <cstdio>


namespace loader
{
	void addUseDirectory(const char *directory);
	void addIncludeDirectory(const char *directory);

	void pushCWD(const char *directory);
	void popCWD();

	void useNameToFilename(char *filename, const char *name);
	void includeNameToFilename(char *filename, const char *name);

	FILE *useFile(const char *filename, char *usedFilename = 0);
	FILE *includeFile(const char *filename, char *usedFilename = 0);
}

#endif

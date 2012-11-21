#ifndef LOADER_H
#define LOADER_H

namespace loader
{
	void addUseDirectory(const char *directory);
	void addIncludeDirectory(const char *directory);

	void pushCWD(const char *directory);
	void popCWD();

	void useNameToFilename(char *filename, const char *name);
	void includeNameToFilename(char *filename, const char *name);

	bool useFile(const char *filename, char *usedFilename = 0);
	bool includeFile(const char *filename, char *usedFilename = 0);
}

#endif

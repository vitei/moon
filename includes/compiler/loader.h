#ifndef COMPILER_LOADER_H
#define COMPILER_LOADER_H

namespace loader
{
	void addUseDirectory(const char *directory);
	void addIncludeDirectory(const char *directory);

	void pushCWD(const char *directory);
	void popCWD();

	void filenameToUseName(char *name, const char *filename);
	void filenameToIncludeName(char *name, const char *filename);

	void useNameToFilename(char *filename, const char *name);
	void includeNameToFilename(char *filename, const char *name);

	bool useFile(const char *filename, char *usedFilename = 0);
	bool includeFile(const char *filename, char *includedFilename = 0);

	bool isAbsolutePath(const char *filename);
}

#endif

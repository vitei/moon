#ifndef LOADER_H
#define LOADER_H

namespace loader
{
	void addClassDirectory(const char *directory);
	void addIncludeDirectory(const char *directory);

	void classNameToFilename(char *filename, const char *className);

	FILE *classFile(const char *filename);
	FILE *includeFile(const char *filename);
}

#endif

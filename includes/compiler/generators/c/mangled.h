#ifndef COMPILER_GENERATORS_C_MANGLED_H
#define COMPILER_GENERATORS_C_MANGLED_H

#include "common.h"

#include <string>

namespace generator
{
	namespace c
	{
		class Mangled
		{
		public:
			Mangled(std::string _declarationName, bool _isImport = false) : declarationName(_declarationName), useName(_declarationName), isImport(_isImport) {}
			Mangled(std::string _declarationName, std::string _useName, bool _isImport = false) : declarationName(_declarationName), useName(_useName), isImport(_isImport) {}

			std::string declarationName;
			std::string useName;
			bool isImport;
		};
	}
}

#endif

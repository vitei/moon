#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>

#ifndef NULL
	#define NULL 0
#endif

#define UNUSED(variable) ((void)&variable)

#define __RANDOM_VARIABLE_NAME_CAT(name, num) name##num                         // This is HORRIBLE...
#define __RANDOM_VARIABLE_NAME(name, num) __RANDOM_VARIABLE_NAME_CAT(name, num) // ...but it works
#define PADDING(numBytes) unsigned char __RANDOM_VARIABLE_NAME(_padding, __LINE__)[numBytes]

#ifdef DEBUG
	#include <iostream>
	#include <cstdio>

	#define ASSERT(test) \
	{ \
		if(!(test)) \
		{ \
			std::cerr << "Exception: File " << __FILE__ << ", Line " << __LINE__ << " \"" << #test << "\"" << std::endl; \
			abort(); \
		} \
	}

	#define ASSERT_LOG(test, ...) \
	{ \
		if(!(test)) \
		{ \
			char __assert_message[1024]; \
			sprintf(__assert_message, __VA_ARGS__); \
			std::cerr << "Exception: File " << __FILE__ << ", Line " << __LINE__ << " \"" << #test << "\", " << __assert_message << std::endl; \
			abort(); \
		} \
	}

	#define __MESSAGE_PRINT(message, ...) \
	{ \
		char __error_message[1024]; \
		sprintf(__error_message, __VA_ARGS__); \
		std::cerr << message << ": " << __error_message << std::endl; \
	}

	#define LOG(...) __MESSAGE_PRINT("LOG", __VA_ARGS__)

	#define WARNING(...) __MESSAGE_PRINT("WARNING", __VA_ARGS__)

	#define ERROR(...) \
	{ \
		__MESSAGE_PRINT("ERROR", __VA_ARGS__); \
		abort(); \
	}
#else
	#define ASSERT(test) { (void)(test); }
	#define ASSERT_LOG(test, ...) { (void)(test); }
	#define LOG(...) {}
	#define WARNING(...) {}
	#define ERROR(...) { exit(EXIT_FAILURE); }
#endif

#endif

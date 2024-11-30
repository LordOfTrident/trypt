#ifndef COMMON_H_HEADER_GUARD
#define COMMON_H_HEADER_GUARD

#include <stdio.h>   /* fprintf, stderr */
#include <stdlib.h>  /* exit, EXIT_FAILURE */
#include <stdarg.h>  /* va_list, va_start, va_end, va_arg */
#include <stdbool.h> /* bool, true, false */
#include <string.h>  /* strcmp */

#define Fatal(...) FatalFrom(__FILE__, __LINE__, __VA_ARGS__)

#ifdef DEBUG
#	define Assert(X, ...)                                         \
	do {                                                          \
		if (!(X))                                                 \
			Fatal("Assertion \"" #X "\" failed: " __VA_ARGS__); \
	} while (0)
#else
#	define Assert(X, ...)
#endif

#define CheckAlloc(PTR)                         \
	do {                                        \
		if (PTR == NULL)                        \
			Fatal("Failed to allocate memory"); \
	} while (0)

#define Die(...)                                \
	do {                                        \
		fprintf(stderr, "Error: " __VA_ARGS__); \
		fprintf(stderr, "\n");                  \
		exit(EXIT_FAILURE);                     \
	} while (0)

void FatalFrom(const char *path, int line, const char *fmt, ...);

#endif

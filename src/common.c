#include "common.h"

void FatalFrom(const char *path, int line, const char *fmt, ...) {
	char    msg[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(msg, sizeof(msg), fmt, args);
	va_end(args);

	fprintf(stderr, "Fatal (%s:%i): %s\n", path, line, msg);
	exit(EXIT_FAILURE);
}

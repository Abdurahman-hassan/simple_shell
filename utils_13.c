#include "main.h"

void custom_log(const char *format, ...)
{
	va_list args;
	char *buffer;

	va_start(args, format);
	vasprintf(&buffer, format, args);
	va_end(args);

	fprintf(stderr, "%s", buffer);
	free(buffer);
}

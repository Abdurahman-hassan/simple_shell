#include "main.h"

/**
 * custom_log - Logs formatted strings to stderr.
 * @format: Format string following printf standards.
 * This function logs messages to the
 * standard error stream, accepting
 * printf-like format specifiers.
 * It handles variable argument lists,
 * formats the message, prints it to stderr,
 * and then frees the allocated
 * memory. The caller does not need
 * to manage memory, as this is handled
 * internally. Note that 'vasprintf' is used
 * for formatting, which is a GNU
 * extension and may affect portability.
 */
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

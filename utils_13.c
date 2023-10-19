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

/**
 * _strstr - Locate a substring within a string.
 * @haystack: The string to search within.
 * @needle: The substring to find.
 *
 * Return: Pointer to the beginning of the located substring,
 * or NULL if the substring is not found.
 */
char *_strstr(char *haystack, const char *needle)
{
    char *start_h = haystack;
    const char *p, *q;

    for (; *start_h != '\0'; start_h++) {
        p = start_h;
        q = needle;
        while (*q != '\0' && *p == *q) {
            p++;
            q++;
        }
        if (*q == '\0')
            return start_h;
    }
    return (NULL);
}
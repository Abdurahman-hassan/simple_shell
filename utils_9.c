#include "main.h"

char **check_separator(char *buf)
{
	char **commands = NULL;

	if (_strchr(buf, ';'))
	{
		commands = split_string(buf, ";");
		return (commands);
	}

	return (NULL);
}
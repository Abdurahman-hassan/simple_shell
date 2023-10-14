#include "main.h"

void exit_(char **path, char *av)
{
	int *_status = get_status();

	if (path[1] != NULL)
	{
		if (is_string(path[1]) == 1 || atoi(path[1]) < 0)
		{
			illegal_number_err(path[1], av);
			free_path(path);
			exit(2);
		}
		
		*_status = atoi(path[1]);
	}

	free_path(path);
	exit(*_status & 255);
}

int is_string(char *str)
{
	int i;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (_isdigit(str[i]) == 1)
			return (-1);
	}

	return (1);
}

/**
 * _isdigit - Checks for a digit (0 through 9).
 *
 * @c: The input to check.
 *
 * Return: 1 if c is a digit.
 *         0 otherwise.
 */

int _isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

void illegal_number_err(char *exit_arg, char *av)
{
	write(STDERR_FILENO, av, _strlen(av));
	write(STDERR_FILENO, ": 1: exit: Illegal number: ", 27);
	write(STDERR_FILENO, exit_arg, _strlen(exit_arg));
	write(STDERR_FILENO, "\n", 1);
}
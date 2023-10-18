#include "main.h"

/**
 * exit_ - a function that handle the exit command status.
 *
 * @path: an array holds the commands enterd by user.
 * @av: Tha program's name.
 *
 * Description: This function handles the built-in Linux command "exit".
 * It exits the shell with the last exit status provided to it as an argument.
 */
void exit_(char **path, char *av)
{
	int *_status = get_status();

	if (path[1] != NULL)
	{
		if (is_string(path[1]) == 1 || _atoi(path[1]) < 0)
		{
			illegal_number_err(path[1], av);
			free_path(path);
			free_path(environ);
			exit(2);
		}

		*_status = _atoi(path[1]);
	}

	free_path(path);
	free_path(environ);
	exit(*_status & 255);
}

/**
 * is_string - Checks if a string contains a digit or not.
 *
 * @str: The string to be checked.
 *
 * Return: If the string does not contain any digits,
 * return 1; otherwise, return -1.
 */
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
 * @c: The input to be checked.
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

/**
 * illegal_number_err - Print an error to the stderr.
 *
 * @exit_arg: The last invalid exit argument.
 * @av: The program's name.
 *
 * Description: When a user enters an invalid exit argument,
 * such as a negative value or a string, the function displays
 * an error message on the screen.
 */
void illegal_number_err(char *exit_arg, char *av)
{
	write(STDERR_FILENO, av, _strlen(av));
	write(STDERR_FILENO, ": 1: exit: Illegal number: ", 27);
	write(STDERR_FILENO, exit_arg, _strlen(exit_arg));
	write(STDERR_FILENO, "\n", 1);
}

/**
 * _atoi - Convert a string to an integer.
 *
 * @s: Pointer of the string.
 *
 * Return: The converted number.
 */
int _atoi(char *s)
{
	int i = 0;
	unsigned int n = 0;
	int sign = 1;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
			sign *= -1;
		else if (s[i] >= '0' && s[i] <= '9')
			n = (n * 10) + (s[i] - '0');
		else if (n > 0)
			break;

		i++;
	}

	return (n * sign);
}

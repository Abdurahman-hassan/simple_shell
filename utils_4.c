#include "main.h"

/**
 * isempty - Checks if a string contains alphabetic characters.
 * @str: The input string to check.
 * This function iterates through the characters
 * of the input string and checks
 * if any of them are alphabetic characters (letters).
 * If at least one alphabetic
 * character is found, it returns 1 to indicate
 * that the string is not empty in
 * terms of alphabetic characters.
 * If no alphabetic characters are found, it
 * returns -1 to indicate that the string is empty
 * in terms of alphabetic characters.
 * Return: 1 if the string contains alphabetic
 * characters, -1 if it does not.
 */

int isempty(char *str)
{
	while (*str != '\0')
	{
		if (_isalpha(*str) == 1)
			return (1);
		str++;
	}

	return (-1);
}

/**
 * _isalpha - Checks for alphabetic character.
 * @c: The character to check.
 * Return: 1 if c is a letter, lowercase or uppercase.
 *         0 otherwise.
 */

int _isalpha(int c)
{
	if (c >= 97 && c <= 122)
		return (1);
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}

/**
 * get_status - Retrieves a pointer to a static
 * integer representing the program's status.
 * This function returns a pointer to a static
 * integer that typically stores the exit
 * status of the program. It can be used to access
 * and modify the program's status
 * across different parts of the code.
 * Return: A pointer to the program's status integer.
 */

int *get_status(void)
{
	static int _status = EXIT_SUCCESS;

	return (&_status);
}

/**
 * _env - Prints the environment variables to standard output.
 *
 * This function iterates through the environment
 * variables and prints each one to
 * the standard output, followed by a newline character.
 * It uses the `environ` variable
 * to access the environment variables.
 */

void _env(void)
{
	int i = 0;

	while (environ && environ[i])
	{
		write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * not_found_err - Prints a "not found"
 * error message to standard error.
 * This function is used to generate
 * a user-friendly error message indicating that a
 * specified command or file was not found.
 * It prints the program's name, the name of
 * the command or file, and an explanatory
 * "not found" message to the standard error.
 * @av: The program's name.
 * @cmd: The command or file not found.
 */

void not_found_err(char *av, char *cmd)
{
	write(STDERR_FILENO, av, _strlen(av));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, cmd, _strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "not found\n", 10);
}

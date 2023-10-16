#include "main.h"

/**
 * _itoa - Convert an integer to a null-terminated string.
 * @value: The integer value to convert.
 * This function converts an integer
 * to a string representation. It first
 * calculates the length of the decimal
 * representation of the integer, then allocates
 * enough memory to hold this representation
 * plus a null-terminator.The integer's digits
 * are then converted one-by-one into characters in the string.
 * The memory allocated for the string
 * should be freed by the caller when no longer needed.
 * Return:
 *      On success, a pointer to the resulting
 *      null-terminated string is returned.
 *      Returns NULL if memory allocation fails.
 */
char *_itoa(int value)
{
	char *str;
	int tmp = value;
	int len = 1, i;

	/* Calculate the length of the number*/
	while (tmp /= 10)
		len++;

	str = (char *)malloc((len + 1) * sizeof(char));
	/* +1 for the null-terminator*/

	if (!str)
		return (NULL); /* memory allocation failed*/

	str[len] = '\0';

	for (i = len - 1; i >= 0; i--)
	{
		str[i] = '0' + (value % 10);
		value /= 10;
	}

	return (str);
}

/**
 * change_directory - Handle the built-in command "cd".
 *
 * @path: The argument provided to the cd command
 * @av: The program's name.
 *
 * Description: First, we get the current working directory and check to see
 * if the command received a no-path argument. The directory is changed to the
 * HOME directory. If the HOME directory is not there, nothing changes; it just
 * remains in the same directory. If the parameter is "-", we switch to the
 * OLDPWD directory, print it to STDOUT, and update the PWD and OLDPWD
 * variables. If the OLDPWD is not present, simply print the current
 * working directory. Otherwise, we check to see if the path is
 * reachable, and if so, we change the directory to the path and update
 * the PWD and OLDPWD variables. If this is not the case, we display
 * an error message on STDERR.
 *
 * Return: If successful, return 0; otherwise, return -1.
 */

int change_directory(char *path, char *av)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (-1);

	if (path == NULL)
	{
		path = _getenv("HOME");

		if (path == NULL)
			return (0);

		if (chdir(path) == -1)
			return (-1);

		_setenv("PWD", path, 1);
		return (0);
	}

	if (_strcmp(path, "-") == 0)
		return (handle_old_dir(cwd));

	if (access(path, F_OK | R_OK) == -1)
	{
		cd_err(cwd, path, av);
		return (-1);
	}

	if (chdir(path) == -1)
		return (-1);

	_setenv("OLDPWD", cwd, 1);
	_setenv("PWD", path, 1);

	return (0);
}

/**
 * cd_err - Print an error message if the STDERR cd command fails to execute
 * and print the current working directory to the STDOUT.
 *
 * @cwd: The current working directory.
 * @path: The argument provided to the cd command.
 * @av: The program's name.
 */

void cd_err(char *cwd, char *path, char *av)
{
	write(STDOUT_FILENO, cwd, _strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	write(STDERR_FILENO, av, _strlen(av));
	write(STDERR_FILENO, ": 1: cd: can't cd to ", 21);
	write(STDERR_FILENO, path, _strlen(path));
	write(STDERR_FILENO, "\n", 1);
}

/**
 * handle_old_dir - function that handles the "cd" command when the
 * argument "-" is provided to it.
 *
 * @cwd: The current working directory.
 *
 * Return: If successful, return 0; otherwise, return -1.
 */

int handle_old_dir(char *cwd)
{
	if (_getenv("OLDPWD") == NULL)
	{
		write(STDOUT_FILENO, cwd, _strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	else
	{
		if (chdir(_getenv("OLDPWD")) == -1)
			return (-1);

		_setenv("PWD", _getenv("OLDPWD"), 1);
		write(STDOUT_FILENO, _getenv("OLDPWD"), _strlen(_getenv("OLDPWD")));
		write(STDOUT_FILENO, "\n", 1);
		_setenv("OLDPWD", cwd, 1);
		return (0);
	}
}

#include "main.h"

/**
 * _strcspn - Calculate the length of the first segment of a string that
 * does not contain any characters from another string.
 * @str: The first string.
 * @reject: The second string.
 * Return: The number of characters in the first string that do not
 * appear in the second string.
 */
size_t _strcspn(const char *str, const char *reject)
{
	size_t len = 0;

	while (*str != '\0')
	{
		if (strchr(reject, *str) != NULL)
		{
			break;
		}
		else
		{
			len++;
		}
		str++;
	}

	return (len);
}

/**
 * allocate_environ - Copy the global array variable environ to the heap.
 *
 * Description: We can't edit the environ array since it isn't dynamically
 * allocated; therefore, in this function, we allocate a heap area and copy
 * the environ array to it to make it easier to modify, and this allocated
 * area will be freed at the end of the program.
 *
 * Return: 0 on success, or -1 if the function failed.
 */
int allocate_environ(void)
{
	int envCount, i = 0;
	char **new_environ = NULL;

	for (envCount = 0; environ[envCount]; envCount++)
		;

	new_environ = malloc(sizeof(char *) * (envCount + 1));
	if (new_environ == NULL)
		return (-1);

	while (environ[i] != NULL)
	{
		new_environ[i] = _strdup(environ[i]);
		i++;
	}

	new_environ[i] = NULL;
	environ = new_environ;

	return (0);
}

/**
 * check_file_in_path - Checks if the file exist in path or not.
 * @buf: a buffer holds the line that passed to the stdin.
 * @path: an array holds the commands enterd by user.
 * @cmd: The command to be executed.
 * @av: The program's name.
 * check the PATH environment variable to see if the
 * "cmd" entered by the user exists or not.
 * Return: The full path of the command, if it's found in the PATH
 * or exit if it's not present.
 */
char *check_file_in_path(char *buf, char **path, char *cmd, char *av)
{
	cmd = search_dir(path[0]); /* handle PATH */

	if (cmd == NULL)
	{
		not_found_err(av, path[0]);
		free(buf);
		free_path(path);
		free_path(environ);
		exit(127);
	}
	else
		return (cmd);
}

/**
 * execute_command - execute a command.
 *
 * @buf: a buffer holds the line that passed to the stdin.
 * @path: an array holds the commands enterd by user.
 * @cmd: The command to be executed.
 *
 * Description: Create a new child process to execute a command.
 * and let the parent process wait until the child process ends execution.
 */
void execute_command(char *buf, char **path, char *cmd)
{
	int *_status = get_status(), status;
	pid_t child_pid;

	child_pid = fork(); /* Create a child process */
	if (child_pid == -1)
	{
		perror("Error on fork");
		free(cmd);
		free(buf);
		free_path(path);
		free_path(environ);
		exit(EXIT_FAILURE); /* Exit if fork failed */
	}

	if (child_pid == 0)
	{
		/* We are in the child process */
		if (execve(cmd, path, environ) == -1)
		{
			perror(cmd);
			free(cmd);
			free(buf);
			free_path(path);
			free_path(environ);
			exit(EXIT_FAILURE); /* Use _exit in child process */
		}
	}
	else
	{
		/* We are in the parent process */
		wait(&status); /* Wait for child process to finish */

		if (WIFEXITED(status))
			*_status = WEXITSTATUS(status);

		free(cmd);
		free_path(path); /* Free path after using it */
	}
}

/**
 * replace_variables - Processes command line arguments, replacing
 * variable patterns with their values. Handles special variables
 * like "$$" and "$?", and replaces environment variables.
 *
 * @args: Null-terminated array of strings, command line arguments.
 * Each might be an argument or a variable needing replacement.
 *
 * @_status: Pointer to an int, the status of the previous command.
 * Used for replacing "$?" with the actual status.
 *
 * Function iterates through 'args'. If an argument starts with '$',
 * it checks for special cases ("$" or "?"). For "$$", it's replaced
 * with the process's PID. For "$?", replaced with '_status'.
 * Otherwise, tries to find the environment variable and replace
 * the argument with its value or an empty string if not found.
 *
 * Original strings being replaced are freed, preventing memory
 * leaks. New strings are allocated in their place.
 */
void replace_variables(char **args, int *_status)
{
	int i, pid;
	char *var_name, *var_value;

	for (i = 0; args[i] != NULL; i++)
	{
		if (args[i][0] == '$')
		{
		/* If var_name is empty or starts with an unsupported character*/
		/* we should treat it as a literal dollar sign.*/
			var_name = args[i] + 1; /* Skip past the '$'*/
			if (*var_name == '\0' ||
					(*var_name == ' ' && var_name[1] == '\0'))
			{
				continue; /* Leave this argument unchanged.*/
			}
			else if (_strcmp(var_name, "$") == 0)
			{
				pid = (int)getpid(); /* Special case of $$ variable*/
				free(args[i]);
				args[i] = _itoa(pid);
			}
			else if (_strcmp(var_name, "?") == 0)
			{
				free(args[i]); /* Special case for the $? variable*/
				args[i] = _itoa(*_status);
			}
			else
			{
				var_value = _getenv(var_name);
				free(args[i]);
				if (var_value != NULL)
				/*The variable is defined so use its value*/
					args[i] = _strdup(var_value);
				else
				/* The variable isn't defined use an empty string */
					args[i] = _strdup("");
			}
		}
	}
}

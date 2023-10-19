#include "main.h"

/**
 * interactive_mode - Enters the interactive mode of the shell
 * @av: The command-line arguments passed to the shell
 * @head: Pointer to the head of the alias list
 *
 * This function allows the user to interact with the shell in
 * an interactive mode. It prompts the user with a shell prompt,
 * reads the input from the user, and processes the input by
 * executing the appropriate commands. The function continues
 * to prompt the user and process the input until the user exits the shell.
 *
 * Return: None
 */
void interactive_mode(char *av, alias_t **head)
{
	char *buf = NULL, *cmd = NULL, **commands;
	int *_status = get_status(), get, i;
	size_t n = 0; /* Allocate buffer size dynamically */

	while (1)
	{
		write(STDOUT_FILENO, "($) ", 4); /* Shell prompt */
		get = _getline(&buf, &n, STDIN_FILENO);
		if (get == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			free(buf);
			exit(*_status); /* Exiting after EOF or error */
		}
		buf[_strcspn(buf, "\n")] = '\0'; /* Remove newline character */
		remove_comment(buf);

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		commands = check_separator(buf);
		if (commands != NULL)
		{
			for (i = 0; commands[i]; i++)
			{
				if (exec(commands[i], av, head, cmd, _status) == -1)
					continue;
			}
			free_path(commands);
		}
		else
		{
			if (exec(buf, av, head, cmd, _status) == -1)
				continue;
		}
	}
	/* This code is unreachable in the current structure, but is good practice */
	free(buf);
	free_list(*head);
}

/**
 * non_interactive_mode - Executes commands in non-interactive mode
 * @av: The command-line arguments passed to the shell
 * @head: Pointer to the head of the alias list
 * This function is responsible for executing commands in non-interactive
 * mode. It reads commands from the standard input and processes them by
 * executing the appropriate commands. The function continues to read
 * and process commands until the end of the input is reached
 * or an error occurs.
 * Return: None
 */
void non_interactive_mode(char *av, alias_t **head)
{
	char *buf = NULL, *cmd = NULL, **commands;
	int *_status = get_status(), get, i;
	size_t n = 0; /* Allocate buffer size dynamically */

	while ((get = _getline(&buf, &n, STDIN_FILENO)) != -1)
	{
		buf[_strcspn(buf, "\n")] = '\0'; /* Remove newline character */
		if (buf[0] == '#')
			continue;

		remove_comment(buf);

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		commands = check_separator(buf);
		if (commands != NULL)
		{
			for (i = 0; commands[i]; i++)
			{
				if (exec(commands[i], av, head, cmd, _status) == -1)
					continue;
			}
			free_path(commands);
		}
		else
		{
			/*path = split_string(buf, " ");*/
			if (exec(buf, av, head, cmd, _status) == -1)
				continue;
		}
	}
	free(buf); /* Free buf at the end (important if getline fails) */
	free_list(*head);
}

/**
 * check_access - Check if a command has access permissions
 * @cmd: The command to check access for
 * @buf: A buffer to store the command
 * @path: An array of paths to search for the command
 * This function checks if the specified command has access
 * permissions by using the access() function. It takes the
 * command, a buffer to store the command, and an array of
 * paths to search for the command. If the command does not
 * have access permissions, it prints a specific error message
 * and frees the memory allocated for the command, buffer, and path array.
 * Return: 0 if the command has access permissions, -1 otherwise.
 */
int check_access(char *cmd, char *buf, char **path)
{
	if (access(cmd, F_OK | X_OK) == -1)
	{
		perror(cmd); /* Give specific error message */
		free(cmd);
		free(buf);
		free_path(path);
		return (-1);
	}

	return (0);
}

/**
 * exec - Executes a command.
 * @buf: The command to execute.
 * @av: The argument vector.
 * @head: A pointer to the head of the alias list.
 * @cmd: The command to execute.
 * @_status: A pointer to the status variable.
 * Return: 0 if successful, -1 otherwise.
 */
int exec(char *buf, char *av, alias_t **head, char *cmd, int *_status)
{
	char **path = split_string(buf, " ");

	if (path == NULL)
		exit(EXIT_FAILURE);

	/* Replace variables in the command */
	replace_variables(path, _status);

	if (check_builtin(path[0]) == 0)
	{
		if (execute_builtin(buf, path, av, head) == 1)
			return (-1);
	}
	else
	{
		replace_alias(head, path);
		cmd = check_file_in_path(buf, path, cmd, av);

		if (check_access(cmd, buf, path) == -1)
			return (-1); /* Go to the next iteration */
		execute_command(buf, path, cmd);
	}

	return (0);
}

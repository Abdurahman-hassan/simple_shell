#include "main.h"

void interactive_mode(char *av)
{
	char *buf = NULL, *cmd = NULL, **path = NULL;
	int get;
	int *_status = get_status();
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

		if (_strcmp(buf, "env") == 0)
			_env();

		path = split_string(buf, " ");

		if (path == NULL)
			exit(EXIT_FAILURE);

		/* Replace variables in the command */
		replace_variables(path, _status);

		if (_strcmp(path[0], "setenv") == 0)
		{
			if (path[1] != NULL && path[2] != NULL)
			{
				if (_setenv(path[1], path[2], 1) == -1)
				{
					perror("Error");
					free(buf);
					free_path(path);
					exit(*_status);
				}
			}
			free_path(path);
			continue;
		}

		if (_strcmp(path[0], "unsetenv") == 0)
		{
			if (_unsetenv(path[1]) == -1)
			{
				perror("Error");
				free(buf);
				free_path(path);
				exit(*_status);
			}
			free_path(path);
			continue;
		}

		if (_strcmp(path[0], "cd") == 0)
		{
			if (change_directory(path[1], av) == -1)
			{
				free(buf);
				free_path(path);
				free_path(environ);
				exit(*_status);
			}
			free_path(path);
			continue;
		}

		if (_strcmp(path[0], "exit") == 0)
		{
			free(buf);
			exit_(path, av);
		}

		cmd = check_file_in_path(buf, path, cmd, av);

		if (access(cmd, F_OK | X_OK) == -1)
		{
			perror(cmd); /* Give specific error message */
			free(cmd);
			free(buf);
			free_path(path);
			continue; /* Go to the next iteration */
		}
		execute_command(buf, path, cmd);
	}
	/* This code is unreachable in the current structure, but is good practice */
	free(buf);
}

void non_interactive_mode(char *av)
{
	char *buf = NULL, *cmd = NULL, **path;
	int get;
	int *_status = get_status();
	size_t n = 0; /* Allocate buffer size dynamically */

	while ((get = _getline(&buf, &n, STDIN_FILENO)) != -1)
	{
		buf[_strcspn(buf, "\n")] = '\0'; /* Remove newline character */
		if (buf[0] == '#')
		{
			continue;
		}
		remove_comment(buf);

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		if (_strcmp(buf, "env") == 0)
		{
			_env();
			continue; /* Skip the rest of the loop for 'env' */
		}

		path = split_string(buf, " ");

		if (path == NULL)
			exit(*_status);

		/* Replace variables in the command */
		replace_variables(path, _status);

		if (_strcmp(path[0], "exit") == 0)
		{
			free(buf);
			exit_(path, av);
		}

		if (_strcmp(path[0], "setenv") == 0)
		{
			if (path[1] != NULL && path[2] != NULL)
			{
				if (_setenv(path[1], path[2], 1) == -1)
				{
					perror("Error");
					free(buf);
					free_path(path);
					exit(*_status);
				}
			}
			free_path(path);
			continue;
		}

		if (_strcmp(path[0], "unsetenv") == 0)
		{
			if (path[1] != NULL)
			{
				if (_unsetenv(path[1]) == -1)
				{
					perror("Error");
					free(buf);
					free_path(path);
					exit(*_status);
				}
			}
			free_path(path);
			continue;
		}

		if (_strcmp(path[0], "cd") == 0)
		{
			if (change_directory(path[1], av) == -1)
			{
				free(buf);
				free_path(path);
				free_path(environ);
				exit(*_status);
			}
			free_path(path);
			continue;
		}

		cmd = check_file_in_path(buf, path, cmd, av);

		if (access(cmd, F_OK | X_OK) == -1)
		{
			perror(cmd); /* Give specific error message */
			free(cmd);
			free(buf);
			free_path(path);
			continue; /* Go to the next iteration */
		}
		execute_command(buf, path, cmd);
	}
	free(buf); /* Free buf at the end (important if getline fails) */
}

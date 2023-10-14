#include "main.h"

void run_interactive(char *av)
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

		buf[strcspn(buf, "\n")] = '\0'; /* Remove newline character */

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		if (strcmp(buf, "env") == 0)
		{
			_env();
			/*continue;*/ /* Skip the rest of the loop for 'env' */
		}

		path = split_string(buf, " ");

		if (path == NULL)
			exit(EXIT_FAILURE);

		if (strcmp(path[0], "exit") == 0)
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

void run_noninteractive(char *av)
{
	char *buf = NULL, *cmd = NULL, **path;
	int get;
	int *_status = get_status();
	size_t n = 0; /* Allocate buffer size dynamically */

	while ((get = _getline(&buf, &n, STDIN_FILENO)) != -1)
	{
		buf[strcspn(buf, "\n")] = '\0'; /* Remove newline character */

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

		if (strcmp(path[0], "exit") == 0)
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
	free(buf); /* Free buf at the end (important if getline fails) */
}

char *check_file_in_path(char *buf, char **path, char *cmd, char *av)
{
	cmd = search_dir(path[0]); /* handle PATH */

	if (cmd == NULL)
	{
		not_found_err(av, path[0]);
		free(buf);
		free_path(path);
		exit(127);
	}
	else
		return (cmd);
}

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
				exit(EXIT_FAILURE); /* Use _exit in child process */
			}
		}
		else
		{
			/* We are in the parent process */
			wait(&status); /* Wait for child process to finish */

			if (WIFEXITED(status))
			{
				*_status = WEXITSTATUS(status);
			}

			free(cmd);
			free_path(path); /* Free path after using it */
		}
}

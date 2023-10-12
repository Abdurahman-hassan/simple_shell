#include "main.h"

#define MAX_CMD_LENGTH 1024

/**
 * main - A simple UNIX command interpreter.
 * @ac: The number of arguments passed to the program.
 * @av: An array of pointers to the arguments.
 * @return 0 on success.
 */
int main(int ac, char **av)
{
	int *_status = get_status();
	(void)ac;

	if (isatty(STDIN_FILENO)) {
		run_interactive(av[0]);
	} else {
		run_noninteractive(av[0]);
	}

	return (*_status);
}

void run_interactive(char *av)
{
	char *buf = NULL, **path;
	pid_t child_pid;
	int get, status;
	size_t n = 0; /* Allocate buffer size dynamically */

	(void)av;
	while (1) {
		write(STDOUT_FILENO, "($) ", 4); /* Shell prompt */

		get = _getline(&buf, &n, STDIN_FILENO);
		if (get == -1) {
			write(STDOUT_FILENO, "\n", 1);
			free(buf);
			exit(EXIT_SUCCESS); /* Exiting after EOF or error */
		}

		buf[strcspn(buf, "\n")] = '\0'; /* Remove newline character */

		if (strcmp(buf, "exit") == 0) {
			free(buf);
			exit(EXIT_SUCCESS); /* Exit command */
		}

		if (strcmp(buf, "env") == 0) {
			_env();
			continue; /* Skip the rest of the loop for 'env' */
		}

		path = split_string(buf, " ");

		if (access(path[0], F_OK | X_OK) == -1) {
			perror(path[0]); /* Give specific error message */
			free(buf);
			free_path(path);
			continue; /* Go to the next iteration */
		}

		child_pid = fork(); /* Create a child process */
		if (child_pid == -1) {
			perror("Error on fork");
			free(buf);
			free_path(path);
			exit(EXIT_FAILURE); /* Exit if fork failed */
		}

		if (child_pid == 0) {
			/* We are in the child process */
			if (execve(path[0], path, NULL) == -1) {
				perror(path[0]);
				free(buf);
				free_path(path);
				exit(EXIT_FAILURE); /* Use _exit in child process */
			}
		} else {
			/* We are in the parent process */
			wait(&status); /* Wait for child process to finish */
		}

		free_path(path); /* Free path after using it */
	}

	/* This code is unreachable in the current structure, but is good practice */
	free(buf);
}

void run_noninteractive(char *av)
{
	char *buf = NULL, **path;
	pid_t child_pid;
	int get, status;
	int *_status = get_status();
	size_t n = 0; /* Allocate buffer size dynamically */

	(void)av;
	while ((get = _getline(&buf, &n, STDIN_FILENO)) != -1) {

		buf[strcspn(buf, "\n")] = '\0'; /* Remove newline character */

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
        {
			free(buf);
			exit(*_status);
		}

		if (strcmp(buf, "exit") == 0) {
			free(buf);
			exit(*_status); /* Exit command */
		}

		if (strcmp(buf, "env") == 0) {
			_env();
			continue; /* Skip the rest of the loop for 'env' */
		}

		path = split_string(buf, " ");

		if (path == NULL)
		{
			exit(EXIT_FAILURE);
		}

		if (access(path[0], F_OK | X_OK) == -1) {
			perror(path[0]); /* Give specific error message */
			free(buf);
			free_path(path);
			continue; /* Go to the next iteration */
		}

		child_pid = fork(); /* Create a child process */
		if (child_pid == -1) {
			perror("Error on fork");
			free(buf);
			free_path(path);
			exit(EXIT_FAILURE); /* Exit if fork failed */
		}

		if (child_pid == 0) {
			/* We are in the child process */
			if (execve(path[0], path, environ) == -1) {
				perror(path[0]);
				free(buf);
				free_path(path);
				exit(EXIT_FAILURE); /* Use _exit in child process */
			}
		} else {
			/* We are in the parent process */
			wait(&status); /* Wait for child process to finish */

			if ( WIFEXITED(status) ) {
        		*_status = WEXITSTATUS(status);
			}
		}

		free_path(path); /* Free path after using it */
	}

	free(buf); /* Free buf at the end (important if getline fails) */
}

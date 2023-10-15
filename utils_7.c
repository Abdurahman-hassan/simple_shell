#include "main.h"

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
 * 
 * @buf: a buffer holds the line that passed to the stdin.
 * @path: an array holds the commands enterd by user.
 * @cmd: The command to be executed.
 * @av: The program's name.
 * 
 * Description: Check the PATH environment variable to see if the 
 * "cmd" entered by the user exists or not.
 * 
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
#include "main.h"

/**
 * main - A simple UNIX command interpreter.
 * @ac: The number of arguments passed to the program.
 * @av: An array of pointers to the arguments.
 * Return: 0 on success.
 */

int main(int ac, char **av)
{
	int *_status = get_status();
	alias_t *head = NULL;

	(void)ac;
	allocate_environ();

	if (isatty(STDIN_FILENO))
	{
		interactive_mode(av[0], &head);
	}
	else
	{
		non_interactive_mode(av[0], &head);
	}

	free_path(environ);
	return (*_status);
}

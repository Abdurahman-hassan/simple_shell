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

	(void)ac;

	if (isatty(STDIN_FILENO))
	{
		run_interactive(av[0]);
	}
	else
	{
		run_noninteractive(av[0]);
	}

	return (*_status);
}

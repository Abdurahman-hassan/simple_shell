#include "main.h"

/**
 * replace_alias - Replace all aliases in the arguments array with
 * their alias_t list values.
 *
 * @head: pointer to the head of the alias_t list.
 * @path: an array holds the commands enterd by user.
 */
void replace_alias(alias_t **head, char **path)
{
	alias_t *node;
	int i;

	for (i = 0; path[i]; i++)
	{
		node = get_node(*head, path[i]);
		if (node)
		{
			free(path[i]);
			path[i] = _strdup(node->value);
		}
	}
}

/**
 * _strpbrk - a function that searches a string for any of a set of bytes.
 *
 * @s: the string to search inside it.
 * @accept: the string to search for one of its letters in s.
 *
 * Return: a pointer to the byte in s that matches one of the bytes
 *		in accept, or NULL if no such byte is found.
 */
char *_strpbrk(char *s, char *accept)
{
	int i = 0, j;

	while (s[i] != '\0')
	{
		j = 0;

		while (accept[j] != '\0')
		{
			if (accept[j] == s[i])
				return (&s[i]);
			j++;
		}

		i++;
	}

	return ('\0');
}

/**
 * _strspn - a function that gets the length of a prefix substring.
 *
 * @s: the string to look for within it.
 * @accept: the string to search for its characters.
 *
 * Return: the number of bytes in the initial segment of s
 *		 which consist only of bytes from accept.
 */
size_t _strspn(char *s, char *accept)
{
	size_t i = 0;
	size_t j;

	while (s[i] != '\0')
	{
		j = 0;

		while (accept[j] != s[i])
		{
			if (accept[j] == '\0')
				return (i);
			j++;
		}

		i++;
	}

	return (i);
}

/**
 * execute_builtin_2 - Executes built-in commands based on
 * the value of path[0].
 * @buf: The input buffer.
 * @path: An array of strings representing the command and its arguments.
 * @av: The name of the program.
 *
 * Return: 1 if a built-in command was executed successfully, -1 otherwise.
 */
int execute_builtin_2(char *buf, char **path, char *av)
{
	int *_status = get_status();

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
		return (1);
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
		return (1);
	}

	if (_strcmp(path[0], "exit") == 0)
	{
		free(buf);
		exit_(path, av);
	}

	return (-1);
}

/**
 * print_node - Prints the name and value of an alias node.
 * @node: A pointer to an alias node.
 *
 * Return: None.
 */
void print_node(alias_t *node)
{
	write(STDOUT_FILENO, node->name, _strlen(node->name));
	write(STDOUT_FILENO, "='", 2);
	write(STDOUT_FILENO, node->value, _strlen(node->value));
	write(STDOUT_FILENO, "'\n", 2);
}

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

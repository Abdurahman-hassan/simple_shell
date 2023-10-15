#include "main.h"

/**
 * _strncmp - Compares the length of two strings.
 * @first_str: The first string.
 * @second_str: The second string.
 * @n: the size of chars
 * Return: If the lengths of the two strings are the same, return 0;
 * otherwise, return the difference between them.
 */

int _strncmp(const char *first_str, const char *second_str, size_t n)
{
	size_t i = 0;
	int res = 0;

	while (i < n)
	{
		if (first_str[i] < second_str[i])
		{
			res = first_str[i] - second_str[i];
			break;
		}
		else if (first_str[i] > second_str[i])
		{
			res = first_str[i] - second_str[i];
			break;
		}
		else if (first_str[i] == '\0' || second_str[i] == '\0')
			break;
		i++;
	}

	return (res);
}

/**
 * _strdup - a function that returns a pointer
 * to a newly allocated space
 * in memory, which contains a copy of the
 * string given as a parameter.
 * @str: the string to be copied.
 * Return: a pointer to the duplicated string
 * or NULL if insufficient memory
 * was available or str = NULL.
 */

char *_strdup(char *str)
{
	char *s;
	unsigned int len = 0, i = 0;

	if (str == NULL)
		return (NULL);

	while (str[i] != '\0')
	{
		len++;
		i++;
	}

	s = malloc((len + 1) * sizeof(char));

	if (s == NULL)
		return (NULL);

	for (i = 0; i < len; i++)
	{
		s[i] = str[i];
	}

	s[i] = '\0';

	return (s);
}

/**
 * search_dir - Searches for a file in directories
 * listed in PATH, returning the full
 * path if found. This function checks
 * each directory in the system's PATH
 * variable for the specified file.
 * If found, it returns the full path;
 * otherwise, it returns NULL.
 * @filename: The file to search for.
 * Return: NULL if not found.
 */

char *search_dir(char *filename)
{
	struct stat st;
	int i = 0;
	char *path = getenv("PATH");
	char **dir = NULL, *file_path = NULL;
	size_t dir_len, file_len;

	if (!filename || (path && _strcmp(path, "") == 0))
		return (NULL);

	if (_strchr(filename, '/') && stat(filename, &st) == 0)
	{
		file_path = _strdup(filename);
		return (file_path);
	}

	if (!path)
		return (NULL);

	file_len = _strlen(filename);
	dir = split_string(path, ":");
	while (dir && dir[i])
	{
		dir_len = _strlen(dir[i]);
		file_path = malloc(sizeof(char) * (dir_len + file_len + 2));
		_strcpy(file_path, dir[i]);
		_strcpy(file_path + dir_len, "/");
		_strcpy(file_path + dir_len + 1, filename);
		if (stat(file_path, &st) == 0)
		{
			free_path(dir);
			return (file_path);
		}
		else
		{
			free(file_path);
			i++;
		}
	}
	free_path(dir);
	return (NULL);
}

/**
 * count_tokens - Counts the number of tokens in a string using a delimiter.
 *
 * @string: The input string to count tokens in.
 * @delim:  The delimiter used to split the string.
 *
 * This function takes an input string and a delimiter and counts the number of
 * tokens (substrings) that would be created by splitting the string based on
 * the delimiter.
 *
 * Return: The number of tokens in the string.
 */

int count_tokens(char *string, char *delim)
{
	int count = 0;
	char *token, *copy;

	if (string == NULL || delim == NULL)
		return (0);

	/* Make a copy of the string to avoid modifying the original */
	copy = _strdup(string);

	if (copy == NULL)
		return (0);

	/* Tokenize the copy and count tokens */
	token = strtok(copy, delim);
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, delim);
	}

	/* Free the copy of the string */
	free(copy);

	return (count);
}

/**
 * split_string - Splits a string into
 * an array of substrings using a delimiter.
 * @string: The input string to be split.
 * @delim:  The delimiter used to split the string.
 *
 * This function takes an input string
 * and a delimiter and splits the string into
 * an array of substrings based on the delimiter.
 * It allocates memory for the substrings
 * and returns an array of pointers to the individual substrings.
 * The array is terminated with a NULL pointer.
 * Memory allocated by this function
 * should be freed when it's no longer needed.
 * Return: An array of pointers to substrings,
 * or NULL if memory allocation fails
 *          or if the input string is empty.
 */

char **split_string(char *string, char *delim)
{
	char **array;
	char *token, *copy;
	int i = 0, j, count;

	count = count_tokens(string, delim);
	if (count == 0)
		return (NULL);

	array = malloc((count + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	/* Make a copy of the string to avoid modifying the original */
	copy = _strdup(string);
	if (copy == NULL)
	{
		free(array);
		return (NULL);
	}
	/* Tokenize the copy and store tokens in the array */
	token = strtok(copy, delim);
	while (token != NULL)
	{
		array[i] = _strdup(token);
		if (array[i] == NULL)
		{
			/* Free allocated memory if an error occurs */
			for (j = 0; j < i; j++)
				free(array[j]);
			free(array);
			free(copy);
			return (NULL);
		}
		i++;
		token = strtok(NULL, delim);
	}
	/* Terminate the array with a NULL pointer */
	array[i] = NULL;
	/* Free the copy of the string */
	free(copy);
	return (array);
}

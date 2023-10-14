#include "main.h"

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
	{
		return (NULL);
	}

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
 * Returns: The number of tokens in the string.
 */
static int count_tokens(char *string, char *delim) {
    int count = 0;
    char *token, *copy;

    if (string == NULL || delim == NULL)
        return 0;

    /* Make a copy of the string to avoid modifying the original */
    copy = _strdup(string);

    if (copy == NULL)
        return 0;

    /* Tokenize the copy and count tokens */
    token = strtok(copy, delim);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delim);
    }

    /* Free the copy of the string */
    free(copy);

    return count;
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
	int i, count, index;
	char *token, *copy, **array;

	/*Check if the string and the delimiter are valid*/
	if (string == NULL || delim == NULL)
	{
		return (NULL);
	}
	/*Make a copy of the string */
	/* to avoid modifying the original*/
	copy = _strdup(string);
	/*Check if the allocation succeeded*/
	if (copy == NULL)
	{
		return (NULL);
	}
	/*Count the number of words in the string*/
	count = 0;
	token = strtok(copy, delim);
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, delim);
	}
	/*Check if the string is empty*/
	if (count == 0)
	{
		free(copy);
		return (NULL);
	}
	/*Restore the copy of the string*/
	_strcpy(copy, string);
	/*restore initial string modified by strtok*/
	/*Allocate an array of pointers to store the words*/
	array = malloc((count + 1) * sizeof(char *));

	/*Check if the allocation succeeded*/
	if (array == NULL)
	{
		free(copy);
		return (NULL);
	}
	/*Split the string and store the words in the array*/
	index = 0;
	token = strtok(copy, delim);
	while (token != NULL)
	{
		/*Allocate memory for each word and copy it*/
		array[index] = _strdup(token);
		/*Check if the allocation succeeded*/
		if (array[index] == NULL)
		{
			/*Free the allocated memory so far*/
			for (i = 0; i < index; i++)
			{
				free(array[i]);
			}
			free(array);
			free(copy);
			return (NULL);
		}

		/*Move to the next word*/
		index++;
		token = strtok(NULL, delim);
	}
	/*Terminate the array with a NULL pointer*/
	array[index] = NULL;
	/*Free the copy of the string*/
	free(copy);
	free(token);
	/*Return the array of words*/
	return (array);
}

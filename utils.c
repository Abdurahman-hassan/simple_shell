#include "main.h"

/**
 * _strlen - A function that returns the length of a string.
 *
 * @s: A pointer of the string.
 *
 * Return: The length of the string.
 */
unsigned int _strlen(char *s)
{
	unsigned int len = 0;

	while (s[len] != '\0')
		len++;

	return (len);
}

/**
 * _strcpy - copies the string pointed to by src,
 *           including the terminating null byte (\0),
 *           to the buffer pointed to by dest.
 *
 * @dest: pointer to the copied string address.
 * @src: pointer the the source string address.
 *
 * Return: the pointer to dest.
 */

char *_strcpy(char *dest, char *src)
{
	int i = -1;

	do {
		i++;
		dest[i] = src[i];
	} while (src[i] != '\0');

	return (dest);

}

/**
 * _strcat - appends the src string to the dest string.
 *
 * @dest: the string to add new string at the end of it.
 *
 * @src: the new string to be added.
 *
 * Return: a pointer to the resulting string dest.
 */

char *_strcat(char *dest, char *src)
{
	int i = 0, j;

	while (dest[i] != '\0')
	{
		i++;
	}

	j = i;

	while (*src != '\0')
	{
		dest[j] = *src;
		src++;
		j++;
	}

	return (dest);
}

/**
 * _strchr - a function that locates a character in a string.
 *
 * @s: the string.
 * @c: the character to be searched.
 *
 * Return: a pointer to the first occurrence of the character c in the string s
 *		if c found, or NULL if the character is not found.
 */

char *_strchr(char *s, char c)
{
	int i = -1;

	do {
		i++;
		if (s[i] == c)
			return (&s[i]);
	} while (s[i] != '\0');

	return ('\0');
}

/**
 * _strcmp - compares two strings.
 *
 * @s1: first string.
 *
 * @s2: second string.
 *
 * Return: 0 if s1 equals s2.
 *         positive number if s1 greater than s2.
 *         negative number if s1 less than s2.
 */

int _strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i] != '\0')
	{
		if (s1[i] != s2[i])
			break;

		i++;
	}

	return (s1[i] - s2[i]);
}

/**
 * _strdup - a function that returns a pointer to a newly allocated space
 * in memory, which contains a copy of the string given as a parameter.
 *
 * @str: the string to be copied.
 *
 * Return: a pointer to the duplicated string, or NULL if insufficient memory
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
 * split_string - A function that splits a string into words.
 * Return: An array of words.
 */
void _env(void)
{
    int i = 0;

    while (environ && environ[i])
    {
        write(STDOUT_FILENO, environ[i], _strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
        i++;
    }
}

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

void not_found_err(char *av, char *cmd)
{
	write(STDERR_FILENO, av, _strlen(av));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, cmd, _strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, "not found\n", 10);
}

char **split_string(char *string, char *delim)
{
    int i, count, index;
    char *token, *copy, **array;

	/*Check if the string and the delimiter are valid*/
	if (string == NULL || delim == NULL)
	{
		return NULL;
	}

    /*Make a copy of the string to avoid modifying the original*/
    copy = _strdup(string);

	/*Check if the allocation succeeded*/
	if (copy == NULL)
	{
		return NULL;
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
		return NULL;
	}

    /*Restore the copy of the string*/
    _strcpy(copy, string); /*restore initial string modified by strtok*/

	/*Allocate an array of pointers to store the words*/
	array = malloc((count + 1) * sizeof(char *));

	/*Check if the allocation succeeded*/
	if (array == NULL)
	{
		free(copy);
		return NULL;
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
			return NULL;
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
	return array;
}

int isempty(char *str)
{
	while (*str != '\0')
	{
		if (_isalpha(*str) == 1)
			return (1);
		str++;
	}

	return (-1);
}

/**
 * _isalpha - Checks for alphabetic character.
 * @c: The character to check.
 *
 * Return: 1 if c is a letter, lowercase or uppercase.
 *         0 otherwise.
 */

int _isalpha(int c)
{
	if (c >= 97 && c <= 122)
		return (1);
	if (c >= 65 && c <= 90)
		return (1);
	return (0);
}

/**
 * free_path - free the path array
 * Function to properly free the path array
 * @path: an array holds the commands enterd by user
 */
void free_path(char** path)
{
	char** current = path;
	while (*current) {
		free(*current); /* free each string */
		current++;
	}
	free(path); /* then free the array */
}


static void (*atexit_functions[ATEXIT_MAX])(void);
static int atexit_count = 0;

/* Our custom atexit function, registering functions to be called at exit */
int my_atexit(void (*function)(void)) {
	if (atexit_count >= ATEXIT_MAX) {
		return -1;  /* Cannot register more functions */
	}
	atexit_functions[atexit_count++] = function;
	return 0;
}

/* Our implementation of the _exit function */
void _exit(int status) {
	/* Flush all open streams */
	if (fflush(NULL) != 0) {
		/* Optionally handle fflush error */
	}

	/* Call functions registered with atexit in reverse order */
	while (atexit_count > 0) {
		atexit_count--;
		(*atexit_functions[atexit_count])();
	}
	/* _exit to system, ending program execution */
	_exit(status);  /* Note: this is the actual _exit system call, it's not recursive */
}

int *get_status(void)
{
	static int _status = EXIT_SUCCESS;

	return (&_status);
}

ssize_t _getline(char **lineptr, size_t *n, int fd) {
	size_t pos = 0;
	ssize_t bytesRead;
	char c;

	/* Validate input parameters */
	if (lineptr == NULL || n == NULL || fd < 0) {
		errno = EINVAL; /* Invalid argument */
		return -1;
	}

	if (*lineptr == NULL) {
		*lineptr = malloc(INITIAL_BUFFER_SIZE);
		if (*lineptr == NULL) {
			errno = ENOMEM; /* Cannot allocate memory */
			return -1;
		}
		*n = INITIAL_BUFFER_SIZE;
	}

	while ((bytesRead = read(fd, &c, 1)) > 0) {
		/* Ensure buffer is large enough, reallocating as necessary */
		if (pos + 1 >= *n) {
			size_t new_size = *n * 2;
			char *new_ptr = realloc(*lineptr, new_size);
			if (new_ptr == NULL) {
				errno = ENOMEM; /* Cannot allocate memory */
				return -1;
			}
			*n = new_size;
			*lineptr = new_ptr;
		}

		(*lineptr)[pos++] = c;

		/* Break on newline */
		if (c == '\n') {
			break;
		}
	}

	if (bytesRead <= 0 && pos == 0) {
		return -1; /* EOF or error */
	}

	(*lineptr)[pos] = '\0'; /* Null-terminate the string */

	return pos; /* Return the number of characters read, not including the null terminator */
}

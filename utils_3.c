#include "main.h"

/**
 * free_path - free the path array
 * Function to properly free the path array
 * @path: an array holds the commands enterd by user
 */
void free_path(char **path)
{
	char **current = path;

	while (*current)
	{
		free(*current); /* free each string */
		current++;
	}
	free(path); /* then free the array */
}


static void (*atexit_functions[ATEXIT_MAX])(void);
static int atexit_count;

/**
 * my_atexit - Register a function to be
 * called at program exit.
 * @function: A pointer to the function to
 * be executed at program exit.
 *
 * This function registers a user-defined function
 * to be executed at program exit.
 * It maintains an internal array of registered functions
 * and allows adding functions
 * until a maximum limit (`ATEXIT_MAX`) is reached.
 * Returns 0 on success, or -1 if
 * the maximum number of registered functions has been reached.
 * Return: 0 on success, -1 if the maximum limit is reached.
 */
int my_atexit(void (*function)(void))
{
	if (atexit_count >= ATEXIT_MAX)
	{
		return (-1);/* Cannot register more functions */
	}
	atexit_functions[atexit_count++] = function;
	return (0);
}

/**
 * _exit - Terminate the program, calling
 * registered exit functions and closing streams.
 * @status: The exit status of the program.
 * This function is a custom implementation of
 * the `_exit` system call. It first flushes
 * all open streams and then calls registered exit
 * functions in reverse order. After
 * performing these tasks, it invokes the actual `_exit`
 * system call to terminate the
 * program execution with the specified exit status.
 */
void _exit(int status)
{
	/* Flush all open streams */
	if (fflush(NULL) != 0)
	{
		/* Optionally handle fflush error */
	}
	/* Call functions registered */
	/* with atexit in reverse order */
	while (atexit_count > 0)
	{
		atexit_count--;
		(*atexit_functions[atexit_count])();
	}
	/* _exit to system, ending program execution */
	_exit(status);
	/* Note: this is the actual _exit system call */
	/* it's not recursive */
}

/**
 * get_status - Retrieves a pointer to a static
 * integer representing the program's status.
 * This function returns a pointer to a static
 * integer that typically stores the exit
 * status of the program. It can be used to access
 * and modify the program's status
 * across different parts of the code.
 * Return: A pointer to the program's status integer.
 */
int *get_status(void)
{
	static int _status = EXIT_SUCCESS;

	return (&_status);
}

/**
 * _getline - Reads a line from a file descriptor
 * and stores it in a dynamically allocated buffer.
 * @lineptr: A pointer to a pointer that will
 * hold the address of the allocated buffer.
 * @n: A pointer to the size of the allocated buffer.
 * @fd: The file descriptor from which to read the line.
 *
 * This function reads a line of text from
 * the specified file descriptor and stores it in a
 * dynamically allocated buffer. It automatically
 * reallocates the buffer if necessary to
 * accommodate longer lines. The function returns
 * the number of characters read, excluding
 * the null terminator, or -1 on error.
 * Return: The number of characters read
 * (excluding the null terminator), or -1 on error.
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	size_t pos = 0;
	ssize_t bytesRead;
	char c;

	/* Validate input parameters */
	if (lineptr == NULL || n == NULL || fd < 0)
	{
		errno = EINVAL; /* Invalid argument */
		return (-1);
	}

	if (*lineptr == NULL)
	{
		*lineptr = malloc(INITIAL_BUFFER_SIZE);
		if (*lineptr == NULL)
		{
			errno = ENOMEM;
			/* Cannot allocate memory */
			return (-1);
		}
		*n = INITIAL_BUFFER_SIZE;
	}

	while ((bytesRead = read(fd, &c, 1)) > 0)
	{
		/* Ensure buffer is large enough */
		/* reallocating as necessary */
		if (pos + 1 >= *n)
		{
			size_t new_size = *n * 2;
			char *new_ptr = realloc(*lineptr,
					new_size);
			if (new_ptr == NULL)
			{
				errno = ENOMEM;
				/* Cannot allocate memory */
				return (-1);
			}
			*n = new_size;
			*lineptr = new_ptr;
		}

		(*lineptr)[pos++] = c;

		/* Break on newline */
		if (c == '\n')
		{
			break;
		}
	}

	if (bytesRead <= 0 && pos == 0)
	{
		return (-1); /* EOF or error */
	}

	(*lineptr)[pos] = '\0'; /* Null-terminate the string */

	/* Return the number of characters read*/
	/* not including the null terminator */
	return (pos);
}

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

/* ###################################################################### */
/* ##                                                                  ## */
/* ##                                                                  ## */
/* ##                          HANDLE FILE                             ## */
/* ##                                                                  ## */
/* ##                                                                  ## */
/* ###################################################################### */


void handle_file(char **av, alias_t **head)
{
	int *_status = get_status(), fd;
	struct stat st;
	size_t capacity, line_count, i;
	ssize_t bytes_read, filesize;
	char *buffer, **lines, *line_start, *ptr, **temp, *cmd;
	char **path, **commands;

	fd = open(av[1], O_RDONLY);
	if (fd == -1) {
		write(STDERR_FILENO, av[0], _strlen(av[0]));
		write(STDERR_FILENO, ": 0: cannot open ", 17);
		write(STDERR_FILENO, av[1], _strlen(av[1]));
		write(STDERR_FILENO, ": No such file\n", 15);
		free_path(environ);
		exit(2);
	}

	if (fstat(fd, &st) == -1) {
		perror("Error getting file size");
		close(fd);
		exit(EXIT_FAILURE);
	}

	filesize = st.st_size;
	buffer = malloc(filesize + 1);
	if (!buffer) {
		perror("Memory allocation failed");
		close(fd);
		exit(EXIT_FAILURE);
	}

	bytes_read = read(fd, buffer, filesize);
	if (bytes_read != filesize) {
		perror("Failed to read entire file");
		free(buffer);
		close(fd);
		exit(EXIT_FAILURE);
	}
	buffer[bytes_read] = '\0';  /* Null-terminate the buffer */

	capacity = 10;  /* initial capacity */
	line_count = 0;
	lines = malloc(capacity * sizeof(char*));
	if (!lines) {
		perror("Memory allocation failed");
		free(buffer);
		close(fd);
		exit(EXIT_FAILURE);
	}

	line_start = buffer;
	for (ptr = buffer; ptr <= buffer + bytes_read; ptr++) {
		if (*ptr == '\n' || ptr == buffer + bytes_read) {  /* If it's a newline or the end of the buffer */
			*ptr = '\0';

			/* Check if the line is empty */
			if (line_start != ptr) {  /* Non-empty line detected */
				/* Resize the array dynamically if required */
				if (line_count == capacity) {
					capacity *= 2;
					temp = realloc(lines, capacity * sizeof(char*));
					if (!temp) {
						perror("Memory reallocation failed");
						for (i = 0; i < line_count; i++) {
							free(lines[i]);
						}
						free(lines);
						free(buffer);
						close(fd);
						exit(EXIT_FAILURE);
					}
					lines = temp;
				}

				lines[line_count] = strdup(line_start);
				line_count++;
			}

			line_start = ptr + 1;
		}
	}

	/* Now we can shrink the lines array to fit the exact number of lines */
	lines = realloc(lines, line_count * sizeof(char*));

	/* Print lines and free memory */
	for (i = 0; i < line_count; i++)
	{
		/*printf("%s\n", lines[i]);*/
		/*######################  START EXECUTE ######################*/

		lines[i][_strcspn(lines[i], "\n")] = '\0';

		remove_comment(lines[i]);

		if (isempty(lines[i]) == -1) /* checks if the string holds spaces only */
			continue;

		/* ========================================== */
		commands = check_separator(lines[i]);

		if (commands != NULL)
		{
			for (i = 0; commands[i]; i++)
			{

				path = split_string(commands[i], " ");

				if (path == NULL)
					exit(EXIT_FAILURE);

				/* Replace variables in the command */
				replace_variables(path, _status);

				if (check_builtin(path[0]) == 0)
				{
					execute_builtin(lines[i], path, av[0], head);
				}
				else
				{
					replace_alias(head, path);
					cmd = check_file_in_path(commands[i], path, cmd, av[0]);

					if (access(cmd, F_OK | X_OK) == -1)
					{
						perror(cmd); /* Give specific error message */
						free(cmd);
						free(lines[i]);
						free_path(path);
						continue; /* Go to the next iteration */
					}
					execute_command(commands[i], path, cmd);
				}
			}
			free_path(commands);

		} /* ============================================================ */
		else
		{
			path = split_string(lines[i], " ");

			if (path == NULL)
				exit(EXIT_FAILURE);

			/* Replace variables in the command */
			replace_variables(path, _status);

			if (check_builtin(path[0]) == 0)
			{
				execute_builtin(lines[i], path, av[0], head);
			}
			else
			{
				replace_alias(head, path);
				cmd = check_file_in_path(lines[i], path, cmd, av[0]);

				if (access(cmd, F_OK | X_OK) == -1)
				{
					perror(cmd); /* Give specific error message */
					free(cmd);
					free(lines[i]);
					free_path(path);
					continue; /* Go to the next iteration */
				}
				execute_command(lines[i], path, cmd);
			}

		}
		free(lines[i]);
	}
	/* This code is unreachable in the current structure, but is good practice */
	free_list(*head);

		/*######################  END EXECUTE ######################*/

	free(lines);
	free(buffer);
	close(fd);
}
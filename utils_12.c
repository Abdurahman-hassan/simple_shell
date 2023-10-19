#include "main.h"

/**
 * handle_file - This function handles a file by opening it,
 * reading its contents, extracting lines, and executing
 * commands based on the lines.
 * @av: A pointer to an array of strings representing command-line arguments.
 * @head: A pointer to a linked list of aliases.
 * Return Type: void
 */
void handle_file(char **av, alias_t **head)
{
	int fd;
	struct stat st;
	size_t capacity, line_count = 0;
	ssize_t bytes_read, filesize;
	char *buffer = NULL, **lines, *line_start;

	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		file_err(av);

	if (fstat(fd, &st) == -1)
		file_interrupt(av, fd, buffer);

	filesize = st.st_size;
	buffer = malloc(filesize + 1);
	if (!buffer)
		file_interrupt(av, fd, buffer);

	bytes_read = read(fd, buffer, filesize);
	if (bytes_read != filesize)
		file_interrupt(av, fd, buffer);

	buffer[bytes_read] = '\0';  /* Null-terminate the buffer */

	capacity = 10;  /* initial capacity */
	lines = malloc(capacity * sizeof(char *));
	if (!lines)
		file_interrupt(av, fd, buffer);

	line_start = buffer;
	line_count = exract_lines(lines, buffer, bytes_read, line_start);

	/* Now we can shrink the lines array to fit the exact number of lines */
	lines = realloc(lines, line_count * sizeof(char *));
	execute_file_cmds(lines, line_count, av, head);

	free_list(*head);
	free(lines);
	free(buffer);
	close(fd);
}

/**
 * file_err - This function is called when there is an error opening a file.
 * It writes an error message to the standard error file descriptor,
 * frees the path environment variable, and exits the program with
 * an exit code of 2.
 * @av: A pointer to an array of strings representing command-line arguments.
 */
void file_err(char **av)
{
	write(STDERR_FILENO, av[0], _strlen(av[0]));
	write(STDERR_FILENO, ": 0: cannot open ", 17);
	write(STDERR_FILENO, av[1], _strlen(av[1]));
	write(STDERR_FILENO, ": No such file\n", 15);
	free_path(environ);
	exit(2);
}

/**
 * file_interrupt - This function is responsible for handling file interrupts.
 * It prints an error message using the name of the program (av[0]),
 * closes the file descriptor (fd), frees the buffer if it is not NULL,
 * and exits the program with a failure status.
 * @av: An array of strings representing the command-line arguments.
 * @fd: An integer representing the file descriptor.
 * @buffer: A pointer to a character representing the buffer.
 */
void file_interrupt(char **av, int fd, char *buffer)
{
	perror(av[0]);
	close(fd);
	if (buffer != NULL)
		free(buffer);
	exit(EXIT_FAILURE);
}

/**
 * execute_file_cmds - This function is responsible for executing commands
 * from a file. It takes an array of lines, the number of lines in the
 * array, an array of command-line arguments, and a pointer to a linked
 * list of aliases. It iterates over each line, removes any trailing
 * newline characters, removes comments from the line, and checks if
 * the line is empty. If the line is not empty, it checks if the
 * line contains a separator character. If it does, it splits the
 * line into multiple commands and executes each command. If the
 * line does not contain a separator character, it executes
 * thesingle command. After executing each command, it frees
 * the memory allocated for the line.
 * @lines: An array of strings representing the lines of commands
 * from the file.
 * @lineCount: A size_t representing the number of lines in the array.
 * @av: An array of strings representing the command-line arguments.
 * @h: A pointer to a pointer to a linked list of aliases.
 * Return: This function does not return a value.
 */
void execute_file_cmds(char **lines, size_t lineCount, char **av, alias_t **h)
{
	size_t i;
	int *_status = get_status();
	char *cmd = NULL, **commands;

	for (i = 0; i < lineCount; i++)
	{
		lines[i][_strcspn(lines[i], "\n")] = '\0';

		remove_comment(lines[i]);

		if (isempty(lines[i]) == -1) /* checks if the string holds spaces only */
			continue;

		commands = check_separator(lines[i]);

		if (commands != NULL)
		{
			for (i = 0; commands[i]; i++)
			{
				if (exec(commands[i], av[0], h, cmd, _status) == -1)
					continue;
			}
			free_path(commands);
		}
		else
		{
			if (exec(lines[i], av[0], h, cmd, _status) == -1)
				continue;
		}
		free(lines[i]);
	}
}

/**
 * exract_lines - This function is responsible for extracting lines
 * from a buffer. It takes an array of strings to store the
 * extracted lines, a buffer containing the data, the number of
 * bytes read from the buffer, a pointer to the start of a line,
 * and a file descriptor. It iterates over each character in the
 * buffer and checks if it is a newline character or the end of
 * the buffer. If it is, it replaces the character with a null
 * terminator and checks if the line is empty. If the line is
 * not empty, it dynamically resizes the array if necessary
 * and stores the line in the array. After processing all the
 * characters, it returns the number of lines extracted.
 * @lines: An array of strings to store the extracted lines.
 * @buf: A pointer to a character representing the buffer containing the data.
 * @rd: A ssize_t representing the number of bytes read from the buffer.
 * @lineStart: A pointer to a character representing the start of a line.
 * @fd: An integer representing the file descriptor.
 * Return: An integer representing the number of lines extracted.
 */
int exract_lines(char **lines, char *buf, ssize_t rd, char *lineStart)
{
	char *ptr, **temp;
	size_t line_count = 0, capacity = 10;

	for (ptr = buf; ptr <= buf + rd; ptr++)
	{
		if (*ptr == '\n' || ptr == buf + rd)
		{  /* If it's a newline or the end of the buffer */
			*ptr = '\0';

			/* Check if the line is empty */
			if (lineStart != ptr)
			{  /* Non-empty line detected */
				/* Resize the array dynamically if required */
				if (line_count == capacity)
				{
					capacity *= 2;
					temp = realloc(lines, capacity * sizeof(char *));
					lines = temp;
				}
				lines[line_count] = strdup(lineStart);
				line_count++;
			}

			lineStart = ptr + 1;
		}
	}
	return (line_count);
}


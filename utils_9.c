#include "main.h"

/**
 * check_separator - Checks for specific
 * separators in a string and splits it.
 * @buf: The input string to check and potentially split.
 * This function checks 'buf' for the presence
 * of a semicolon (';'), '&&', or '||'. If found,
 * it splits 'buf' into a null-terminated
 * array of strings (commands) divided by
 * these separators. It uses 'split_string' for this operation.
 * Return: Null-terminated array of
 * commands if a separator is found, or NULL if not.
 */
char **check_separator(char *buf)
{
    char **commands = NULL;
    const char *separators[] = { ";", "&&", "||", NULL };
    int i;

    for (i = 0; separators[i]; ++i)
    {
        if (_strstr(buf, separators[i]))
        {
            commands = split_string(buf, separators[i]);
            if (commands) {
                break;
            }
        }
    }

    return (commands);
}
/**
 * _Strtok_r - Splits a string into
 * tokens; reentrant version of strtok.
 * @str: The string to tokenize; subsequent calls should pass NULL.
 * @delim: The delimiter characters.
 * @saveptr: A pointer to a char* variable
 * that stores context between successive calls.
 * This function is a reentrant (thread-safe)
 * version of strtok. It finds the next
 * token in 'str' delimited by characters
 * in 'delim'. It's designed to be called multiple times
 * to obtain successive tokens from the same string.
 * 'saveptr' is used to maintain context
 * between these calls and should be retained
 * unmodified between successive calls.
 * If 'str' is NULL, the saved pointer in 'saveptr' is used instead.
 * Return: Pointer to the next token, or NULL if there are no more tokens.
 */
char *_Strtok_r(char *str, const char *delim, char **saveptr)
{
	char *token_start, *token_end;

	if (!str)
		str = *saveptr;

	/* Skip leading delimiters */
	str += _strspn(str, (char *)delim);
	if (*str == '\0')
	{
		*saveptr = str;
		return (NULL);
	}

	/* Initialize token start and find the end of the token */
	token_start = str;
	str = _strpbrk(token_start, (char *)delim);
	if (str == NULL)
	{
		*saveptr = _strchr(token_start, '\0');
	}
	else
	{
		token_end = str;
		*token_end = '\0';
		*saveptr = token_end + 1;
	}

	return (token_start);
}

/**
 * free_tokens - Frees each string in an
 * array of strings, then the array itself.
 * @tokens: Null-terminated array of strings.
 * Iterates through 'tokens', an array of strings,
 * freeing each string, then frees the array itself.
 * The array must be null-terminated, signifying the end
 * with a NULL pointer. Used to avoid
 * memory leaks by releasing no-longer-needed
 * string arrays created by functions like split_string.
 */
void free_tokens(char **tokens)
{
	char **tmp = tokens;

	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(tokens);
}

/**
 * check_builtin - Determine whether the passed command is built-in or not.
 *
 * @cmd: The passed command.
 *
 * Return: if the command is built-in return 0,
 * otherwise return -1.
 */
int check_builtin(char *cmd)
{
	int i;
	char *builtin_commands[] = {"env", "exit", "setenv", "unsetenv",
	"cd", "alias"};

	for (i = 0; i < 6; i++)
	{
		if (_strcmp(builtin_commands[i], cmd) == 0)
			return (0);
	}

	return (-1);
}

/**
 * execute_builtin - Execute a built-in command.
 *
 * @buf: a buffer holds the line that passed to the stdin.
 * @path: an array holds the commands enterd by user.
 * @av: The program's name.
 * @head: pointer to the head of the alias_t list.
 *
 * Return: if the command executed successfully return 1,
 * otherwise return -1.
 */
int execute_builtin(char *buf, char **path, char *av, alias_t **head)
{
	int *_status = get_status();
	int ret;

	replace_variables(path, _status);

	if (_strcmp(path[0], "alias") == 0)
	{
		handle_alias(path, head);
		free_path(path);
		return (1);
	}

	if (_strcmp(path[0], "env") == 0)
	{
		_env();
		free_path(path);
		return (1);
	}

	if (_strcmp(path[0], "setenv") == 0)
	{
		if (path[1] != NULL && path[2] != NULL)
		{
			if (_setenv(path[1], path[2], 1) == -1)
			{
				perror("Error");
				free_path(path);
				exit(*_status);
			}
		}
		free_path(path);
		return (1);
	}

	ret = execute_builtin_2(buf, path, av);

	return (ret);
}

#include "main.h"

/**
 * check_separator - Checks for a specific
 * separator in a string and splits it.
 * @buf: The input string to check and potentially split.
 * This function checks 'buf' for the presence
 * of a semicolon (';'). If found,
 * it splits 'buf' into a null-terminated
 * array of strings (commands) divided by
 * this separator. It uses 'split_string' for this operation.
 * Return: Null-terminated array of
 * commands if ';' is found, or NULL if not.
 */
char **check_separator(char *buf)
{
	char **commands = NULL;

	if (_strchr(buf, ';'))
	{
		commands = split_string(buf, ";");
		return (commands);
	}

	return (NULL);
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
	str += strspn(str, delim);
	if (*str == '\0')
	{
		*saveptr = str;
		return (NULL);
	}

	/* Initialize token start and find the end of the token */
	token_start = str;
	str = strpbrk(token_start, delim);
	if (str == NULL)
	{
		*saveptr = strchr(token_start, '\0');
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

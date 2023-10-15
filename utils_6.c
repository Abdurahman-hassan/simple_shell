#include "main.h"

/**
 * _setenv - Initialize a new environment variable, or modify an existing one.
 * @name: The name of the environment variable.
 * @value: The value of the environment variable.
 * @overwrite: A trigger number determines whether or not
 * the variable value should be overwritten.
 * Description: The function accepts the name and value of the environment
 * variable and determines whether or not the name already exists as an
 * environment variable. A new variable with the provided name and value
 * is created and added to the environment array. If the name already exists,
 * we look at the overwrite value. If it is not zero, the old value of the
 * name is overwritten with a new one; if the overwritten value is zero,
 * nothing is changed.
 * Return: 0 on success, or -1 if the function failed.
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	size_t name_len = _strlen((char *)name), i, envCount = 0;
	int var_index;
	char *val = NULL, *newVar = NULL, **newEnviron = NULL;

	if (!name || name_len == 0 || !value)
		return (-1);

	for (i = 0; i < name_len; i++)
	{
		if (name[i] == '=')
			return (-1);
	}
	for (envCount = 0; environ[envCount]; envCount++)
		;
	newVar = concat((char *)name, "=", (char *)value);
	if (newVar == NULL)
		return (-1);
	val = _getenv(name);
	/*Case of existing enviroment variable*/
	/*if overwrite != 0, overwrite the value*/
	if (val != NULL && overwrite != 0)
	{
		var_index = get_var_index((char *)name, val);
		free(environ[var_index]);
		environ[var_index] = newVar;
		return (0);
	}
	else
	{
		newEnviron = realloc(environ, ((envCount + 2) * sizeof(char *)));
		if (newEnviron == NULL)
		{
			free(newVar);
			return (-1);
		}
		newEnviron[envCount] = newVar;
		newEnviron[envCount + 1] = NULL;
		environ = newEnviron;
	}
	return (0);
}

/**
 * _unsetenv - Removes a variable from the environment array.
 * @name: The name of the variable.
 * Description: The function looks for the
 * variable "name" in the environ array.
 * It will remove it and reindex the environment array if it detects it.
 * If the name cannot be discovered, simply return zero.
 * Return: Always 0.
 */
int _unsetenv(const char *name)
{
	int index = -1, i, j;

	for (i = 0; environ[i]; i++)
	{
		if (_strncmp(environ[i], (char *)name, _strlen((char *)name)) == 0
				&& environ[i][_strlen((char *)name)] == '=')
		{
			index = i;
			free(environ[i]);
			break;
		}
	}
	if (index == -1)
		return (0);

	for (j = index; environ[j]; j++)
		environ[j] = environ[j + 1];

	return (0);
}

/**
 * concat - Concatenate two strings with a delimiter
 * between them to one string
 * @first_str: The first string.
 * @delim: The delimiter.
 * @second_str: The second string.
 * Description: The function takes the two strings,
 * creates a new memory area, and copies the two
 * strings and the delimiter into it.
 * Return: If the function fails, the new string is returned, else it is NULL.
 */
char *concat(char *first_str, char *delim, char *second_str)
{
	size_t first_len = _strlen(first_str);
	size_t second_len = _strlen(second_str);
	char *new_str = malloc(sizeof(char) * (first_len + second_len + 2));

	if (new_str == NULL)
		return (NULL);

	_strcpy(new_str, first_str);
	_strcpy(new_str + first_len, delim);
	_strcpy(new_str + first_len + 1, second_str);

	return (new_str);
}

/**
 * get_var_index - Find the index of an environment variable.
 * @name: Name of the variable.
 * @val: Value of the variable.
 * Return: If the variable is found, return its index; else, return -1.
 */
int get_var_index(char *name, char *val)
{
	int i;
	char *var = concat(name, "=", val);

	for (i = 0; environ[i]; i++)
	{
		if (_strcmp(environ[i], var) == 0)
		{
			free(var);
			return (i);
		}
	}

	free(var);
	return (-1);
}

/**
 * _getenv - Obtains the value of a variable in the environment array.
 * @name: The name of the variable.
 * Return: If the variable is found, its value is returned;
 * otherwise, NULL is returned.
 */
char *_getenv(const char *name)
{
	int i = 0;
	int len = _strlen((char *)name);

	while (environ[i])
	{
		if (_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			return (&environ[i][len + 1]);
		}
		i++;
	}

	return (NULL);
}

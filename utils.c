#include "main.h"

/**
 * _strlen - A function that returns the length of a string.
 * @s: A pointer of the string.
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
 * @dest: pointer to the copied string address.
 * @src: pointer the the source string address.
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
 * @dest: the string to add new string at the end of it.
 * @src: the new string to be added.
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
 * @s: the string.
 * @c: the character to be searched.
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

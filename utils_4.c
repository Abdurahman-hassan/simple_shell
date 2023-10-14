#include "main.h"

/**
 * isempty - Checks if a string contains alphabetic characters.
 *
 * @str: The input string to check.
 *
 * This function iterates through the characters
 * of the input string and checks
 * if any of them are alphabetic characters (letters).
 * If at least one alphabetic
 * character is found, it returns 1 to indicate
 * that the string is not empty in
 * terms of alphabetic characters.
 * If no alphabetic characters are found, it
 * returns -1 to indicate that the string is empty
 * in terms of alphabetic characters.
 * Return: 1 if the string contains alphabetic
 * characters, -1 if it does not.
 */
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

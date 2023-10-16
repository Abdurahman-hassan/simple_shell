#include "main.h"

/**
 * _itoa - Convert an integer to a null-terminated string.
 * @value: The integer value to convert.
 * This function converts an integer
 * to a string representation. It first
 * calculates the length of the decimal
 * representation of the integer, then allocates
 * enough memory to hold this representation
 * plus a null-terminator.The integer's digits
 * are then converted one-by-one into characters in the string.
 * The memory allocated for the string
 * should be freed by the caller when no longer needed.
 * Return:
 *      On success, a pointer to the resulting
 *      null-terminated string is returned.
 *      Returns NULL if memory allocation fails.
 */
char *_itoa(int value)
{
	char *str;
	int tmp = value;
	int len = 1, i;

	/* Calculate the length of the number*/
	while (tmp /= 10)
		len++;

	str = (char *)malloc((len + 1) * sizeof(char));
	/* +1 for the null-terminator*/

	if (!str)
		return (NULL); /* memory allocation failed*/

	str[len] = '\0';

	for (i = len - 1; i >= 0; i--)
	{
		str[i] = '0' + (value % 10);
		value /= 10;
	}

	return (str);
}

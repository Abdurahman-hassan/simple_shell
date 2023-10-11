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
 * split_string - A function that splits a string into words.
 * Return: An array of words.
 */
void _env(void)
{
    int i = 0;
    extern char **environ;

    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]);
        i++;
    }
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
    copy = strdup(string);

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
    strcpy(copy, string); /*restore initial string modified by strtok*/

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
        array[index] = strdup(token);

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

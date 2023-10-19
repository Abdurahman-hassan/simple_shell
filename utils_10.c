#include "main.h"

/**
 * add_node_end - a function that adds a new node at the end of a alias_t list.
 *
 * @head: pointer to the head of the list.
 * @name: The name of the alias.
 * @value: The value of the alias.
 *
 * Return: the address of the new element, or NULL if it failed.
 */
alias_t *add_node_end(alias_t **head, const char *name, const char *value)
{
	alias_t *newNode = malloc(sizeof(alias_t));
	alias_t *curr = *head;

	if (newNode == NULL)
		return (NULL);

	newNode->name = _strdup((char *)name);
	newNode->value = _strdup((char *)value);
	newNode->next = NULL;

	if (curr != NULL)
	{
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = newNode;
	}
	else
		*head = newNode;

	return (newNode);
}

/**
 * print_list - a function that prints all the elements of a alias_t list.
 *
 * @h: pointer to the head of the list.
 *
 * Return: the number of nodes.
 */
size_t print_list(const alias_t *h)
{
	unsigned int count = 0;
	alias_t *curr = (alias_t *)h;

	while (curr != NULL)
	{
		count++;
		if (curr->name == NULL)
			printf("[0] (nil)\n");
		else
		{
			write(STDOUT_FILENO, curr->name, _strlen(curr->name));
			write(STDOUT_FILENO, "='", 2);
			write(STDOUT_FILENO, curr->value, _strlen(curr->value));
			write(STDOUT_FILENO, "'\n", 2);
		}

		curr = curr->next;
	}

	return (count);
}

/**
 * free_list - a function that frees a alias_t list.
 *
 * @head: pointer to the head of the list.
 */
void free_list(alias_t *head)
{
	alias_t *currNode;

	while (head)
	{
		currNode = head;
		head = head->next;
		free(currNode->name);
		free(currNode->value);
		free(currNode);
	}
}

/**
 * handle_alias - Handle the built-in command "alias".
 *
 * @path: an array holds the commands enterd by user.
 * @head: pointer to the head of the alias_t list.
 *
 * Description: This function checks the arguments passed with
 * the alias command. If no arguments are provided, we print the
 * stored aliases; otherwise, we cycle through the arguments and
 * check if the argument is in the form "name." We look for this
 * name and print it in the format "name=value." If the argument
 * is in the form "name=value," we look for the name in the alias
 * list and update its value; if the name is not found in the alias
 * list, we add it as a new node to the list.
 *
 * Return: always 0 (success).
 */
int handle_alias(char **path, alias_t **head)
{
	int i;
	alias_t *node;
	char *name, *val, *next_ptr;

	if (path[1] == NULL)
	{
		if (print_list(*head) == 0)
			write(STDOUT_FILENO, "No aliases found\n", 17);
		return (0);
	}

	for (i = 1; path[i]; i++)
	{
		if (_strchr(path[i], '=') == NULL)
		{
			node = get_node(*head, path[i]);
			if (node != NULL)
				print_node(node);
		}
		else
		{
			name = _Strtok_r(path[i], "=", &next_ptr);
			val = _Strtok_r(NULL, "=", &next_ptr);
			node = get_node(*head, val);

			if (node != NULL)
				val = node->value;

			node = get_node(*head, path[i]);
			if (node != NULL)
			{
				free(node->value);
				node->value = _strdup(val);
			}
			else
				add_node_end(head, name, val);
		}
	}

	return (0);
}

/**
 * get_node - Look for a specific node in the alias_t list.
 *
 * @head: pointer to the head of the alias_t list.
 * @name: the name of the node that we are looking for.
 *
 * Return: If we found the node, return a pointer to it; else, return NULL.
 */
alias_t *get_node(alias_t *head, char *name)
{
	alias_t *curr = head;

	while (curr != NULL)
	{
		if (_strcmp(curr->name, name) == 0)
			return (curr);

		curr = curr->next;
	}

	return (NULL);
}

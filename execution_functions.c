#include "main.h"

void interactive_mode(char *av, alias_t **head)
{
	char *buf = NULL, *cmd = NULL, **path = NULL, **commands;
	int get, i;
	int *_status = get_status();
	size_t n = 0; /* Allocate buffer size dynamically */

	while (1)
	{
		write(STDOUT_FILENO, "($) ", 4); /* Shell prompt */
		get = _getline(&buf, &n, STDIN_FILENO);
		if (get == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			free(buf);
			exit(*_status); /* Exiting after EOF or error */
		}

		buf[_strcspn(buf, "\n")] = '\0'; /* Remove newline character */

		remove_comment(buf);

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		/* ========================================== */
		commands = check_separator(buf);

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
					execute_builtin(buf, path, av, head);
				}
				else
				{
					replace_alias(head, path);
					cmd = check_file_in_path(commands[i], path, cmd, av);

					if (access(cmd, F_OK | X_OK) == -1)
					{
						perror(cmd); /* Give specific error message */
						free(cmd);
						free(buf);
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
			path = split_string(buf, " ");

			if (path == NULL)
				exit(EXIT_FAILURE);

			/* Replace variables in the command */
			replace_variables(path, _status);

			if (check_builtin(path[0]) == 0)
			{
				execute_builtin(buf, path, av, head);
			}
			else
			{
				replace_alias(head, path);
				cmd = check_file_in_path(buf, path, cmd, av);

				if (access(cmd, F_OK | X_OK) == -1)
				{
					perror(cmd); /* Give specific error message */
					free(cmd);
					free(buf);
					free_path(path);
					continue; /* Go to the next iteration */
				}
				execute_command(buf, path, cmd);
			}

		}
	}
	/* This code is unreachable in the current structure, but is good practice */
	free(buf);
	free_list(*head);
}

void non_interactive_mode(char *av, alias_t **head)
{
	char *buf = NULL, *cmd = NULL, **path, **commands;
	int get, i;
	int *_status = get_status();
	size_t n = 0; /* Allocate buffer size dynamically */

	while ((get = _getline(&buf, &n, STDIN_FILENO)) != -1)
	{
		buf[_strcspn(buf, "\n")] = '\0'; /* Remove newline character */
		if (buf[0] == '#')
		{
			continue;
		}
		remove_comment(buf);

		if (isempty(buf) == -1) /* checks if the string holds spaces only */
			continue;

		commands = check_separator(buf);

		/* ################################################################## */

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
					if (execute_builtin(buf, path, av, head) == 1)
						continue;
				}
				else
				{
					replace_alias(head, path);
					cmd = check_file_in_path(commands[i], path, cmd, av);

					if (access(cmd, F_OK | X_OK) == -1)
					{
						perror(cmd); /* Give specific error message */
						free(cmd);
						free(buf);
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
			path = split_string(buf, " ");

			if (path == NULL)
				exit(EXIT_FAILURE);

			/* Replace variables in the command */
			replace_variables(path, _status);

			if (check_builtin(path[0]) == 0)
			{
				if (execute_builtin(buf, path, av, head) == 1)
					continue;
			}
			else
			{
				replace_alias(head, path);
				cmd = check_file_in_path(buf, path, cmd, av);

				if (access(cmd, F_OK | X_OK) == -1)
				{
					perror(cmd); /* Give specific error message */
					free(cmd);
					free(buf);
					free_path(path);
					continue; /* Go to the next iteration */
				}
				execute_command(buf, path, cmd);
			}

		}
		 /* ################################################################## */
	}
	free(buf); /* Free buf at the end (important if getline fails) */
	free_list(*head);
}
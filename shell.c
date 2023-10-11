#include "main.h"

#define MAX_CMD_LENGTH 1024
/**
 * main - A simple UNIX command interpreter.
 * @ac: The number of arguments passed to the program.
 * @av: An array of pointers to the arguments.
 * @return 0 on success.
 */
int main(int ac, char **av)
{
    (void)ac;

    if (isatty(STDIN_FILENO)) {
        run_interactive(av[0]);
    } else {
        run_noninteractive(av[0]);
    }

    return (0);
}

void run_interactive(char *av)
{
    char *buf = NULL, **path;
    pid_t child_pid;
    int get, status;
    size_t n = 10;

    while (1) {
        /*Print the custom shell prompt*/
        write(STDOUT_FILENO, "($) ", 4);

        get = getline(&buf, &n, stdin);
        if (get == -1)
        {
            write(STDOUT_FILENO, "\n", 2);
            exit(EXIT_FAILURE);
        }

        /*Remove the trailing newline character*/
        buf[strcspn(buf, "\n")] = '\0';

        /*Exit the shell when the user types 'exit'*/
        if (strcmp(buf, "exit") == 0) {
            exit(EXIT_FAILURE);
        }

        if (strcmp(buf, "env") == 0)
            _env();

        path = split_string(buf, " ");

        if (access(path[0], F_OK & X_OK) == -1)
        {
            printf("%s: %s\n", av, strerror(errno));
            continue;
        }

        child_pid = fork(); /*Create a child process*/
        if (child_pid == -1) {
            printf("%s: %s\n", av, strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) { /*Inside the child process*/

            /*Execute the command*/
            if (execve(path[0], path, NULL) == -1)
            {
                printf("%s: %s\n", av, strerror(errno));
                continue;
            }
        } else {
            wait(&status);  /*Make the parent process wait until the child completes*/
        }
    }
}

void run_noninteractive(char *av)
{
    char *buf = NULL, **path;
    pid_t child_pid;
    int get, status;
    size_t n = 10;

    (void) av;

    get = getline(&buf, &n, stdin);
    if (get == -1)
    {
        write(STDOUT_FILENO, "\n", 2);
        exit(EXIT_FAILURE);
    }

    /*Remove the trailing newline character*/
    buf[strcspn(buf, "\n")] = '\0';

    /*Exit the shell when the user types 'exit'*/
    if (strcmp(buf, "exit") == 0) {
        exit(EXIT_FAILURE);
    }

    if (strcmp(buf, "env") == 0)
        _env();

    path = split_string(buf, " ");

    if (access(path[0], F_OK | X_OK) == -1)
    {
        printf("%s: %s\n", av, strerror(errno));
    }

    child_pid = fork(); /*Create a child process*/
    if (child_pid == -1) {
        printf("%s: %s\n", av, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { /*Inside the child process*/

        /*Execute the command*/
        if (execve(path[0], path, NULL) == -1)
        {
            printf("%s: %s\n", av, strerror(errno));
        }
    } else {
        wait(&status);  /*Make the parent process wait until the child completes*/
    }
}

#ifndef SIMPLE_SHELL_MAIN_H
#define SIMPLE_SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

void run_interactive(char *av);
void run_noninteractive(char *av);
unsigned int _strlen(char *s);
void _env(void);
char **split_string(char *string, char *delim);

#endif /* SIMPLE_SHELL_MAIN_H */


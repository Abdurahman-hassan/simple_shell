#ifndef SIMPLE_SHELL_MAIN_H
#define SIMPLE_SHELL_MAIN_H

/* Assuming a max of 32 functions that can be registered with atexit. */
#define ATEXIT_MAX 32
#define INITIAL_BUFFER_SIZE 128

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;

void run_interactive(char *av);
void run_noninteractive(char *av);
unsigned int _strlen(char *s);
void _env(void);

char **split_string(char *string, char *delim);
int count_tokens(char *string, char *delim);

void free_path(char **path);
int my_atexit(void (*function)(void));
void _exit(int status);
int *get_status(void);
int isempty(char *str);
int _isalpha(int c);

ssize_t _read_chars(char **lineptr, size_t *n, int fd);
ssize_t _getline(char **lineptr, size_t *n, int fd);

char *search_dir(char *filename);
void not_found_err(char *av, char *cmd);
char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char *_strchr(char *s, char c);
int _strcmp(char *s1, char *s2);

#endif /* SIMPLE_SHELL_MAIN_H */

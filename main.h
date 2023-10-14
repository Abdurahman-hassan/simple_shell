#ifndef SIMPLE_SHELL_MAIN_H
#define SIMPLE_SHELL_MAIN_H

/* Assuming a max of 32 functions that can be registered with atexit. */
#define ATEXIT_MAX 32
#define INITIAL_BUFFER_SIZE 128
#define MAX_CMD_LENGTH 1024

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


/* ============ INTERACTIVE FUNCTION ============== */
void run_interactive(char *av);

/* ============ NON_INTERACTIVE FUNCTION ============== */

void run_noninteractive(char *av);

/* ===========  UTILS ============ */

unsigned int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char *_strchr(char *s, char c);
int _strcmp(char *s1, char *s2);

/* =============  UTILS_2 ============ */

char *_strdup(char *str);
char *search_dir(char *filename);
int count_tokens(char *string, char *delim);
char **split_string(char *string, char *delim);

/* =============  UTILS_3 =============== */

void free_path(char **path);
int my_atexit(void (*function)(void));
void _exit(int status);
ssize_t _read_chars(char **lineptr, size_t *n, int fd);
ssize_t _getline(char **lineptr, size_t *n, int fd);

/* ============== UTILS_4 ================ */

int *get_status(void);
int isempty(char *str);
int _isalpha(int c);
void not_found_err(char *av, char *cmd);
void _env(void);


void execute_command(char *buf, char **path, char *cmd);
char *check_file_in_path(char *buf, char **path, char *cmd, char *av);

#endif /* SIMPLE_SHELL_MAIN_H */

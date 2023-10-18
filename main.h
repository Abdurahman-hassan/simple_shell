#ifndef SIMPLE_SHELL_MAIN_H
#define SIMPLE_SHELL_MAIN_H

/* Assuming a max of 32 functions that can be registered with atexit. */
#define ATEXIT_MAX 32
#define INITIAL_BUFFER_SIZE 128
#define MAX_CMD_LENGTH 1024
#define PATH_MAX 1024

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
void interactive_mode(char *av);

/* ============ NON_INTERACTIVE FUNCTION ============== */

void non_interactive_mode(char *av);

/* ===========  UTILS ============ */

unsigned int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char *_strchr(char *s, char c);
int _strcmp(char *s1, char *s2);

/* =============  UTILS_2 ============ */

int _strncmp(const char *first_str, const char *second_str, size_t n);
char *_strdup(char *str);
char *search_dir(char *filename);
int count_tokens(char *string, const char *delim);
char **split_string(char *string, const char *delim);

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

/* ============== UTILS_5 ================ */

void exit_(char **path, char *av);
int is_string(char *str);
int _isdigit(int c);
void illegal_number_err(char *exit_arg, char *av);
int _atoi(char *s);

/* ============== UTILS_6 ================ */

int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
char *concat(char *first_str, char *delim, char *second_str);
int get_var_index(char *name, char *val);
char *_getenv(const char *name);

/* ============== UTILS_7 ================ */

size_t _strcspn(const char *str, const char *reject);
int allocate_environ(void);
char *check_file_in_path(char *buf, char **path, char *cmd, char *av);
void execute_command(char *buf, char **path, char *cmd);
void replace_variables(char **args, int *status);

/* ============= UTILS_8 ================= */

char *_itoa(int value);
int change_directory(char *path, char *av);
void cd_err(char *cwd, char *path, char *av);
int handle_old_dir(char *cwd);
void remove_comment(char *str);

/* ============= UTILS_9 ================= */

char **check_separator(char *buf);
char *_Strtok_r(char *str, const char *delim, char **saveptr);
void free_tokens(char **tokens);

#endif /* SIMPLE_SHELL_MAIN_H*/


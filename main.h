#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>


extern char **environ; /*Declare environ*/

#define MAX_INPUT_SIZE 100
#define MAX_ARG_COUNT 32
#define MAX_PATH_LENGTH 4096


int main(void);
void execute_commands(char *input);
int command_exists(const char *command);
int _strlen(const char *s);
void _strncat(char *dest, const char *src, size_t n);
void execute_command_with_args(char *command);
int _strcmp(char *s1, char *s2);
void print_environment(void);
const char *_getenv(const char *name);
const char *_strchr(const char *s, char c);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strdup(const char *s);
char *_strcpy(char *dest, const char *src);
void _memcpy(void *dest, const void *src, size_t n);
int construct_full_path(char *command, char *path_copy, char *full_path, int *full_path_len);
int construct_argument_array(char *command, char **argv);

#endif /* MAIN_H */

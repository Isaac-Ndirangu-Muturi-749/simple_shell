#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>
#include <fcntl.h>

extern char **environ; /*Declare environ*/

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 32
#define MAX_PATH_LENGTH 1024

void execute_cmds(char *input);
void execute_cmd_with_args(char *cmd);
void handle_child_exit(char **argv);
int construct_argument_array(char *cmd, char **argv);

int cmd_exists(const char *cmd);
int _strlen(const char *s);
void _strncat(char *dest, const char *src, size_t n);

int _strcmp(char *s1, char *s2);
void print_environment(void);
const char *_getenv(const char *name);
const char *_strchr(const char *s, char c);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strdup(const char *s);
char *_strcpy(char *dest, const char *src);
void _memcpy(void *dest, const void *src, size_t n);
int make_fullpath(char *cmd, char *pathcpy, char *fullpath, int *fullpath_len);

void execute_cmd(char *fullpath, char **argv);


void interactive_mode(void);
void batch_mode(char *filename);
int main(void);
void remove_comment(char *input);


#endif /* MAIN_H */

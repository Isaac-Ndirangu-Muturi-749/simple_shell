#include "main.h"

/**
 * execute_commands - Execute the commands entered by the user.
 *
 * @input: The input string containing the command.
 */
void execute_commands(char *input)
{
	char *command = strtok(input, " ");
	pid_t child_pid;

	if (command == NULL)
	{
		return; /* Empty line, do nothing */
	}

	/* Check if the user entered "exit" */
	if (_strcmp(command, "exit") == 0)
	{
		exit(EXIT_SUCCESS); /* Exit the shell */
	}
	else if (_strcmp(command, "env") == 0)
	{
		print_environment(); /* Call the function to print environment */
		return;
	}

	child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (child_pid == 0)
	{
		/* Child process */
		execute_command_with_args(command);
	}
	else
	{
		/* Parent process */
		wait(NULL); /* Wait for the child process to finish */
	}
}

/**
 * execute_command_with_args - Execute a command with its arguments.
 *
 * @command: The command to execute.
 */
void execute_command_with_args(char *command)
{
	char **argv = (char **)malloc(sizeof(char *) * MAX_ARG_COUNT);
	int arg_count = 0;
	char *arg;

	argv[arg_count++] = command;

	arg = strtok(NULL, " ");
	while (arg != NULL && arg_count < (MAX_ARG_COUNT - 1))
	{
		argv[arg_count++] = arg;
		arg = strtok(NULL, " ");
	}

	argv[arg_count] = NULL; /* Null-terminate the argument array */

	if (command_exists(command))
	{
		if (execve(command, argv, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		perror("Command not found");
		exit(EXIT_FAILURE);
	}

	free(argv);
}

/**
 * command_exists - Check if a command exists in the PATH.
 *
 * @command: The command to check for.
 *
 * Return: 1 if the command exists, 0 otherwise.
 */
int command_exists(const char *command)
{
	/* Retrieve the PATH environment variable */
	const char *path_env = _getenv("PATH");
	char *path_env_copy;
	char *path;
	char full_path[MAX_PATH_LENGTH];

	if (path_env == NULL)
	{
		return (0); /* PATH is not set, the command cannot exist */
	}
	path_env_copy = _strdup(path_env); /* Create a copy of path_env */
	if (path_env_copy == NULL)
	{
		perror("strdup");
		return (0);
	}

	path = strtok(path_env_copy, ":");
	while (path != NULL)
	{
		int path_length = _strlen(path);
		int command_length = _strlen(command);

		if (path_length + command_length + 2 < MAX_PATH_LENGTH)
		{
			full_path[0] = '\0';
			_strncat(full_path, path, path_length);
			_strncat(full_path, "/", 1);
			_strncat(full_path, command, command_length);

			if (access(full_path, X_OK) == 0)
			{
				free(path_env_copy); /* Free the allocated memory */
				return (1); /* Command exists in this directory */
			}
		}
		path = strtok(NULL, ":");
	}
	free(path_env_copy); /* Free the allocated memory */
	return (0); /* Command not found in any directory in PATH */
}

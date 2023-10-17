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
 * construct_argument_array - Construct an argument array for the command.
 *
 * @command: The command to execute.
 * @argv: Pointer to the argument array to be constructed.
 *
 * Return: the number of arguments in the array.
 */
int construct_argument_array(char *command, char **argv)
{
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
	return (arg_count);
}

/**
 * execute_command - Execute a command with its arguments.
 *
 * @full_path: The full path to the command.
 * @argv: The argument array.
 */
void execute_command(char *full_path, char **argv)
{
	if (execve(full_path, argv, environ) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
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
	char *path_copy;
	char full_path[MAX_PATH_LENGTH];
	int full_path_len = 0;

	construct_argument_array(command, argv);

	if (_strchr(command, '/') != NULL)
	{
		execute_command(command, argv);
	}
	else
	{
		const char *path = _getenv("PATH");

		if (path == NULL)
		{
			perror("getenv");
			exit(EXIT_FAILURE);
		}

		path_copy = _strdup(path);
		if (path_copy == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}

		if (construct_full_path(command, path_copy, full_path, &full_path_len) == 0)
		{
			execute_command(full_path, argv);
		}
		else
		{
			perror("Command not found");
			exit(EXIT_FAILURE);
		}
		free(path_copy);
	}
	free(argv);
}

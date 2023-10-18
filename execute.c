#include "main.h"



/**
 * execute_cmds - Execute the commands entered by the user.
 *
 * @input: The input string containing the command.
 */
void execute_cmds(char *input)
{
	char *cmd = strtok(input, " ");
	pid_t child_pid;

	if (cmd == NULL)
	{
		return; /* Empty line, do nothing */
	}

	/*Remove comment lines (starting with '#')*/
	remove_comment(input);

	/* Check for built-in commands */
	if (_strcmp(cmd, "exit") == 0)
	{
		free(input);
		exit(0);
	}
	else if (_strcmp(cmd, "env") == 0)
	{
		print_environment(); /* Call the function to print the environment */
		return;
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(2);
	}
	if (child_pid == 0)
	{
		/* Child process */
		execute_cmd_with_args(cmd);
		exit(127); /* This line is executed only if the command is not found */
	}
	else
	{
		/* Parent process */
		wait(NULL); /* Wait for the child process to finish */
	}
}




/**
 * execute_cmd_with_args - Execute a cmd with its arguments.
 *
 * @cmd: The cmd to execute.
 */
void execute_cmd_with_args(char *cmd)
{
	char **argv = (char **)malloc(sizeof(char *) * MAX_ARG_COUNT);
	char *pathcpy;
	char fullpath[MAX_PATH_LENGTH];
	int fullpath_len = 0;

	construct_argument_array(cmd, argv);

	if (_strchr(cmd, '/') != NULL)
	{
		execute_cmd(cmd, argv);
	}
	else
	{
		const char *path = _getenv("PATH");

		if (path == NULL)
		{
			perror("getenv");
			exit(EXIT_FAILURE);
		}

		pathcpy = _strdup(path);
		if (pathcpy == NULL)
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}

		if (make_fullpath(cmd, pathcpy, fullpath, &fullpath_len) == 0)
		{
			execute_cmd(fullpath, argv);
		}
		else
		{
			/*Pass the argv array to handle_child_exit*/
			handle_child_exit(argv);
			free(pathcpy);
		}
		free(argv);
	}
}


/**
 * handle_child_exit - Common error handling for child process exit status.
 * @argv: The argument array.
 */
void handle_child_exit(char **argv)
{
	int status;

	wait(&status);

	if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
	{
		perror(argv[0]);
		exit(127);
	}
}


/**
 * construct_argument_array - Construct an argument array for the cmd.
 *
 * @cmd: The cmd to execute.
 * @argv: Pointer to the argument array to be constructed.
 *
 * Return: the number of arguments in the array.
 */
int construct_argument_array(char *cmd, char **argv)
{
	int arg_count = 0;
	char *arg;

	argv[arg_count++] = cmd;
	arg = strtok(NULL, " ");

	while (arg != NULL && arg_count < (MAX_ARG_COUNT - 1))
	{
		argv[arg_count++] = arg;
		arg = strtok(NULL, " ");
	}

	argv[arg_count] = NULL; /*Null-terminate the argument array*/
	return (arg_count);
}

/**
 * execute_cmd - Execute a cmd with its arguments.
 *
 * @fullpath: The full path to the cmd.
 * @argv: The argument array.
 */
void execute_cmd(char *fullpath, char **argv)
{
	if (execve(fullpath, argv, environ) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

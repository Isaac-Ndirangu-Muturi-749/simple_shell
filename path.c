#include "main.h"

/**
 * construct_full_path - Construct the full path to the command.
 *
 * @command: The command to execute.
 * @path_copy: A copy of the PATH environment variable.
 * @full_path: The buffer to store the full path.
 * @full_path_len: Pointer to the full path length.
 *
 * Return: 0 on success, -1 on failure.
 */
int construct_full_path(char *command, char *path_copy, char *full_path, int *full_path_len)
{
	char *dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		int dir_length = _strlen(dir);
		int command_length = _strlen(command);

		if (*full_path_len + dir_length + command_length + 1 >= MAX_PATH_LENGTH)
		{
			perror("Path length exceeds the maximum path length");
			return (-1);
		}

		_memcpy(full_path + *full_path_len, dir, dir_length);
		*full_path_len += dir_length;

		full_path[*full_path_len] = '/';
		(*full_path_len)++;

		_memcpy(full_path + *full_path_len, command, command_length);
		*full_path_len += command_length;

		full_path[*full_path_len] = '\0';

		if (access(full_path, X_OK) == 0)
		{
			return (0); /*Full path constructed successfully*/
		}
		dir = strtok(NULL, ":");
		*full_path_len = 0;  /* Reset the length for the next iteration */
	}

	return (-1); /*Command not found*/
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

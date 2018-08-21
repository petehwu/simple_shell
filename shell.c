#include "holberton.h"
/**
 * main - main shell function
 * Return: always 0
 */

int main(int argc __attribute__((unused)), char **argv, char **env)
{
	pid_t child_pid;
	size_t input_count = 0;
	int stat, check_path = 0;
	char *buff = NULL, *buff_tk1 = NULL;
	char **buff_tk = NULL;
	size_t br = 0;
	ssize_t read;
	char *path = NULL;
	
	
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$> ", 3);
		input_count++;
		read = getline(&buff, &br, stdin);
		if (read == 0)
			break;
		if (read == -1)
		{
			break;
		}
		if (buff && buff[0] == '\n')
			continue;
		buff_tk = create_arg_list(buff_tk, buff, " \n");
		if (get_builtin_func(buff_tk)(buff_tk, env, buff) == 0)
			continue;
		check_path = access(buff_tk[0], X_OK);
		if (check_path == -1)
		{
			path = _getenv("PATH", env);
			buff_tk1 = _strdup(buff_tk[0]);
			buff_tk[0] = path_helper(path, buff_tk[0]);
		}
		if (buff_tk[0] == NULL)
		{
			write(STDOUT_FILENO, argv[0], _strlen(argv[0]));
			write(STDOUT_FILENO, ": ", 2);
			write(STDOUT_FILENO, num_to_str(input_count), _strlen(num_to_str(input_count)));
			write(STDOUT_FILENO, ": ", 2);
			write(STDOUT_FILENO, buff_tk1, _strlen(buff_tk1));
			write(STDOUT_FILENO, ": not found\n", 12);
			continue;
		}
		child_pid = fork();
		if (child_pid == -1)
		{
			perror(argv[0]);
		}
		if (child_pid == 0)
		{
			if (execve(buff_tk[0], buff_tk, NULL) == -1)
			{
				perror(argv[0]);
				free(buff_tk);
				free(buff);
				break;
			}
			
		}
		else
		{
			wait(&stat);
			if (check_path == -1)
				free(buff_tk[0]);
			free(buff_tk1);
			free(buff_tk);
			free(buff);				
			buff_tk = NULL;
			buff = NULL;
			path = NULL;	
		}
	}
	return (0);
}

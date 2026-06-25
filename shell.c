#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10

int main()
{
	char *prompt = NULL;
	char *args[10]; // Max arguments can be taken is ten
	char *token;

	while (1)
	{
		// initialize / refressh the args buffer for the next command
		int token_count = 0;
		for (int i = 0; i < MAX_ARGS; i++)
		{
			args[i] = NULL;
		}

		// take user prompt
		prompt = readline("shshell> ");

		// exit
		if (strcmp(prompt, "exit") == 0)
		{
			free(prompt);
			exit(0);
		}

		else
		{
			// tokenize prompt
			token = strtok(prompt, " ");

			while (token != NULL)
			{
				if (token_count < 10)
				{
					args[token_count] = token;
					token_count++;
				}
				token = strtok(NULL, "- ");
			}

			// handle cd command
			if (strcmp(args[0], "cd") == 0)
			{
				if (args[1] == NULL)
				{
					chdir(getenv("HOME"));
				}
				else
				{
					if (chdir(args[1]) == -1)
					{
						perror("couldn't change directory");
					}
				}
				continue;
			}

			// detect for pipe
			bool pipe_symbol = false;
			int pipe_index = 0;

			for (int i = 0; i < MAX_ARGS; i++)
			{
				if (args[i] != NULL)
				{
					if (strcmp(args[i], "|") == 0)
					{
						pipe_symbol = true;
						pipe_index = i;
					}
				}
			}


			if (pipe_symbol == true)
			{
			// separate commands
			char *first_command[pipe_index + 1];
			char *second_command[MAX_ARGS - pipe_index - 1];

			for (int i = 0; i < pipe_index ; i++)
			{
				first_command[i] = args[i];
			}
			first_command[pipe_index] = NULL;
			
			for (int i = 0; i < MAX_ARGS - pipe_index - 1; i++)
			{
				second_command[i] = args[i + pipe_index + 1];
			}
			second_command[MAX_ARGS - pipe_index - 1] = NULL;
	

			int pipefd[2];
			pipe(pipefd);

			pid_t first_pid = fork();

			if (first_pid == 0)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
				execvp(first_command[0], first_command);
				exit(1);
			}

			pid_t second_pid = fork();

			if (second_pid == 0)
			{
				close(pipefd[1]);
				dup2(pipefd[0], STDIN_FILENO);
				close(pipefd[0]);
				execvp(second_command[0], second_command);
				exit(1);

			}

			close(pipefd[0]);
			close(pipefd[1]);
			wait(NULL);
			wait(NULL);
			continue;

		}
		else
		{
			// detect redirection
			bool output_redirection = false;
			int redirection_index = 0;
			char *filename = NULL;

			for (int i = 0; i < MAX_ARGS; i++)
			{
				if (args[i] != NULL && (strcmp(args[i], ">") == 0))
				{
					output_redirection = true;
					redirection_index = i;
					filename = args[i + 1];
					break;
				}
			}

			pid_t pid = fork();

			if (pid < 0)
			{
				perror("Fork failed, exiting program");
				exit(1);
			}

			if (pid == 0)
			{

				// handle output redirection
				if (output_redirection == true)
				{
					// open redirection file and replace file descriptor
					int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
					if (fd == -1)
					{
						perror("failed to open file\n");
					}

					dup2(fd, STDOUT_FILENO);
					close(fd);

					// clean token array
					for (int i = redirection_index; i < 10; i++)
					{
						args[i] = NULL;
					}
				}

				
				execvp(args[0], args);
				perror("failed to execute ");
				continue;
			}

			if (pid > 0)
			{
				wait(NULL);
			}
			continue;
		}

		free(prompt);
	}
	}
}

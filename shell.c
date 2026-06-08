#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  char *prompt = NULL;
  char *args[10]; // Max arguments can be taken is ten
  char *token;
  
  while(1)
    {
      // initialize / refressh the args buffer for the next command
      int token_count = 0;
      for (int i = 0; i < 10; i++)
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
	  token = strtok(prompt, "- ");

	  while (token != NULL)
	    {
	      if (token_count < 10)
		{
		  args[token_count] = token;
		  token_count++;
		}
		  token = strtok(NULL, "-");
	       
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
	  	 
	  pid_t pid = fork();

	  if (pid < 0)
	    {
	      perror("Fork failed, exiting program");
     	      exit(1);
	    }

	  if (pid == 0)
	    {
	      for (int i = 0; i < 10; i++)
		{
		  printf("args[%d]: %s\n", i, args[i]);
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

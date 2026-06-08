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
      int token_count = 0;

      for (int i = 0; i < 10; i++)
	{
	  args[i] = NULL;
	}
      
      prompt = readline("shshell> ");
      
      if (strcmp(prompt, "exit") == 0)
	{
	  free(prompt);
	  exit(0);
	}
      
      else
	{
	  // printf("%s\n", prompt);
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

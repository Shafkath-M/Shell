#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

int main()
{
  char *prompt = NULL;
  char *args;
  while(1)
    {
      prompt = readline("shshell> ");

      if (prompt == "")
	{
	  printf("empty prompt\n");
	  continue;
	}
      
      else if (strcmp(prompt, "exit") == 0)
	{
	  free(prompt);
	  exit(0);
	}
      
      else
	{
	  printf("%s\n", prompt);
	  args = strtok(prompt, "-");

	  while (args != NULL)
	    {
	      printf("%s\n", args);
	      args = strtok(NULL, "-");
	    }
	  continue;
	}

      free(prompt);
      
    }
}

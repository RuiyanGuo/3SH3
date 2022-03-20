//Ruiyan Guo 400256752
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

// signal handler
void signal_handler ( int signo )
{
  if (signo == SIGINT)
   {
     printf("Ctrl-C will not terminate [3SH3_Lab1.c]\n");
   }
  if (signo == SIGQUIT)
   {
     printf("Ctrl-\\ will not terminate [3SH3_Lab1.c]\n");
   }
  if (signo == SIGTSTP)
   {
     printf("Ctrl-Z was pressed! [3SH3_Lab1.c]\n");
   }
  if (signo == SIGALRM)
   {
     printf("30 seconds exceeded, exiting automatically...\n");
     exit(0);
   }
}

int main (void)
{
  char userinput[1000]; //For userinput, increase the length of it to prevent overflow
  char compare[5] = "stop"; //For comparison purpose
  int result;
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTSTP, signal_handler);
  signal(SIGALRM, signal_handler); //Trigger the alarm automatically after 30 seconds
  alarm(30); //Total execution time of 30 seconds

 while(1)
  {
    printf("User input needed. (Notice, the program will terminates after 30s since the execution starts)\n");
    scanf("%s", userinput);
    strcpy(compare, "stop");//copy the stop to the compare again, in case the contents in input overflow to the compare array
    result = strcmp(userinput, compare); //Compare to check if user input is "stop"
    if (result == 0)
     {
      printf("User input string stop detected, exiting...\n");
      exit(0); //Exit when user input "stop"
     }
  }
  return 0;
}

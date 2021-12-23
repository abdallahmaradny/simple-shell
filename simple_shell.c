#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <time.h>
void SIGNAL_HANDLER();
int main()
{

while(1)
{
//printf("begin \n");
    char line[200];
//scanning command from user
    fgets (line, 100, stdin);
  

char *TOKENS[100];
//the parameters that the strtok will seperate on
char PARAM_TO_SEPERATE_ON[ ]=" ";
char *argument;
argument = strtok(line," ");
int position=0;
//splitting the command and saving the output 
while(argument != NULL)
{
   TOKENS[position]=argument;
    position++;
    argument=strtok(NULL," ");
 }
//removing the new line and placing null
TOKENS[position-1][strlen(TOKENS[position-1])-1]='\0';
TOKENS[position]=NULL;
//printf("%s",TOKENS[0]);
char **first=TOKENS;
int pid;

//comparing if it is exit terminate the shell
if(strcmp(TOKENS[0],"exit")==0)
{
exit(0);
}

if(TOKENS[1]!=NULL){
//if it is a background process

if(strcmp(TOKENS[1],"&")==0)
{
TOKENS[1]=NULL;
  pid = fork();
  if (pid == 0) {//process for child
    if (execvp(TOKENS[0], first) == -1) {//fail in execution
printf("Error in child execution for background \n");
    }
  } else if (pid < 0) {//fail in forking
    printf("Error in fork in background \n");
  }
else{
//signal when the child finishes its process and terminates
signal (SIGCLD,SIGNAL_HANDLER);
}
}
else {
pid = fork();
  if (pid == 0) {//process for child
    if (execvp(TOKENS[0], first) == -1) {//fail in execution
printf("Error in child execution for background \n");
    }
  } else if (pid < 0) {//fail in forking
    printf("Error in fork in background \n");
  }
else{
//signal when the child finishes its process and terminates
signal (SIGCLD,SIGNAL_HANDLER);
}
}
}


//if foreground process
else
{
  pid = fork();
  if (pid == 0) {//child process
    if (execvp(TOKENS[0], first) == -1) {
printf("Error in child execution in foreground \n");
    }
  } else if (pid < 0) {
    // Error forking
    printf("Error in fork in foreground \n");
  }
else
{//parent waits for the child to complete its process
      wait(NULL);
} 

   } }

    return 0;
}

void SIGNAL_HANDLER(int signum)
{

FILE * file = fopen("SHELL.txt","a");
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char * Message = "\nA Child was dead at ";
    fputs(Message,file);
    fputs(asctime(timeinfo),file);
fclose(file);
}

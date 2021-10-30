/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE        80 /* 80 chars per line, per command */



void AddToHistory(char* inp[]);
void DisplayHistory();


char History[MAX_LINE*2];
int  LenHistory = 0;

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;
        
        while (should_run)
        {   
            char UserInp[MAX_LINE];
            printf("osh>");
            fgets(UserInp, MAX_LINE, stdin);
            if (UserInp[0] == '\n')
            {
                continue;
            }
            char* inp1 = strtok(UserInp,"\n");
            int ind = strlen(inp1);
            int Concurrency = 0;
            char* inp;
            if (inp1[ind-1] == '&')
            {
                inp1[ind-2] = '&';
                inp = strtok(inp1,"&");

                Concurrency = 1;
            }
            else
            {
                inp = strtok(UserInp,"\n");
            }
            if (strcmp(inp, "!!") == 0)
            {
                DisplayHistory();
                continue;
            }
            pid_t childPid = fork();

            int i1 = 0;
            char *p = strtok (inp1, " ");

            while (p != NULL)
            {
                args[i1++] = p;
                p = strtok (NULL, " ");
            }
            args[i1] = NULL;
            
            if (childPid == 0)          //Child Process
            {
                int childStatus = execvp(args[0], args);
                if (childStatus == -1)
                {
                    printf("Invalid Input! Try again.\n");
                }
                should_run = 0;
            }
            else if (childPid > 0)         //Parent Process
            {
                if (Concurrency ==0)
                {
                    // printf("this is the ampersand condition\n");
                    int v1;
                    pid_t childWaitPid = waitpid(childPid, v1, 0);

                }
                AddToHistory(args);
            }

            for (int i = 0; i > MAX_LINE; i++)
            {
                UserInp[i] = "\0";
                args[i] = "\0";
            }
            fflush(stdout);
            fflush(stdin);        
            /**
             * After reading user input, the steps are:
             * (1) fork a child process
             * (2) the child process will invoke execvp()
             * (3) if command includes &, parent and child will run concurrently
             */
        }
    
    return 0;
}

void AddToHistory(char* inp[])
{
    int L = LenHistory + strlen(inp);
    History[L] = '\n';

    int j =0;
    int nn= 0 ;
    for (int i = LenHistory; i <L; i++)
    {
        History[i] = inp[j];
        nn++;
        j++;
    }
    LenHistory = LenHistory + nn;
}

void DisplayHistory()
{
    int L = strlen(History);

    // for (int i = 0; i <L; i++)
    // {
        printf("%s", History);
    // }
}
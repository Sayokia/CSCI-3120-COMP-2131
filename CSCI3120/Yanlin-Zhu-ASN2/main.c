#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
    char history[10][MAX_LINE];
    int pids[10];
    char *token;
    char userInput[MAX_LINE];
    char *args[MAX_LINE/2 + 1]; /* an array of character strings */
    int should_run = 1; /* flag to determine when to exit program */

    // initialize array
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 41; j++) {
            history[i][j] = '\0';
        }
    }
    while (should_run) {
        printf("CSCI3120>");
        fflush(stdout);

        // clean up args by removing previous command
        int p =0;
        while(args[p] != NULL){
            args[p] = NULL;
            p++;
        }

        //read in user input
        fgets(userInput, MAX_LINE, stdin);
        //remove "\n" from the user input
        strtok(userInput, "\n");

        /*
         * There are two types of command:
         * 1. special command: history
         * 2. special command with !
         * 2.1 special command !!
         * 2.2 sepcial command !N
         * 3. Linux command
         */
        // (1) Handle special command history
        if (strcmp(userInput,"history")==0){
            printf("%s\t", "ID");
            printf("%s\t\t", "PID");
            printf("%s\n", "Command");
            for(int m=0; m<10;m++){

                if (history[m][0]!= '\0'){
                    printf("%d\t", m+1);
                    printf("%d\t", pids[m]);
                    printf("%s",history[m]);
                }
                printf("\n");
            }
        }

        // (2) special cases that contain ! in command
        //The usage of strcpy is learned from https://www.programiz.com/c-programming/library-function/string.h/strcpy
        //(2.1) special case !!
        else if (strcmp(userInput,"!!")==0){
            if(history[0][0] !='\0'){
                strcpy(userInput,history[0]);
                // goto statement is learned from https://www.programiz.com/c-programming/c-goto-statement
                goto process;
            } else{
                printf("No commands in history\n");
            }

        }

        //(2.2) special case with !N (1<=N<=10)
        // Check if the user input contains ! as the starter and have more than 2 characters
        else if(userInput[0] == '!' && strlen(userInput)>=2){
            int N = 0;
            // get int from user input string
            //the solution of retrieving int from a string is referred to  https://www.programiz.com/c-programming/c-goto-statement
            for (int i = 0; userInput[i]!='\0' ; i++) {
                if (userInput[i]<='9' && userInput[i]>='0'){
                    N = N*10 + userInput[i]-'0';
                }
            }

            if(N<=0 || N>10){
                printf("Such a command is Not in history\n");
            }
            else{
                if(history[N-1][0] =='\0'){
                    printf("Such a command is Not in history\n");
                } else{
                    strcpy(userInput,history[N-1]);
                    goto process;
                }
            }

        }
        //handle normal linux command
        else{
            process: /*  Set up a goto label for special command !! or !N
 * This is to make sure that command start with ! is not included in the history  */

            // add command to history no matter it is valid or not (as per Sep. 30th update)

            // I am using a for loop and strcpy to move previous command backward
            for (int i = 9; i >0 ; i--) {
                strcpy(history[i],history[i-1]);
            }
            strcpy(history[0],userInput);




            // separate user input by space
            token = strtok(userInput, " ");
            // put all tokens into args[] array
            int i = 0;
            while(token != NULL)
            {
                args[i++] = token;

                token = strtok(NULL, " ");
            }

            /* Creating	the	child process and executing	the	command
             * via the	child process is referred to Professor Ye's slides*/

            // fork a child process
            pid_t pid = fork();

            // put pid into pids array and using a for loop to move previous pid backward
            for (int o = 9; o >0 ; o--) {
                pids[o] = pids[o-1];
            }
            pids[0] = pid;

            int execvp_status,wait_status;
            // error occurred
            if (pid <0) {
                printf("fork failed\n");
                return 1;
            }
                // child process
            else if(pid == 0) {
                execvp_status = execvp(args[0], args);
                if(execvp_status==-1) {
                    printf("Invalid command\n");
                    exit(1);
                }
            } else{
                // parent process
                // wait for child process to complete
                wait(&wait_status);
            }





        }
        }



    return 0;
}


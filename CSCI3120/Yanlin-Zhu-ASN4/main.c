#include <stdio.h>
#include <string.h>
#include <stdlib.h>


FILE *f;
int size = 0;
int arrivalTime[50], burstTime[50];

void FCFS();

void RR();

void NPSJF();

void PSJF();

int main() {
    // Read in file

    f = fopen("TaskSpec.txt","r");
    char line[10];


    char *token;
    while(fgets(line,10,f)!=NULL){
        //ignore taskname
        token = strtok(line,",");
        //read in next token
        token = strtok(NULL, ",");
        // put the arrival time into the arrival time int list at respective index
        int a = atoi(token);
        arrivalTime[size] = a;
        //read in next token
        token = strtok(NULL, ",");
        //put the burst time into the burst time int list at the respective index
        int b = atoi(token);
        burstTime[size] = b;
        size++;
    }

    //create the output file
    f = fopen("Output.txt", "a");
    //put result of FCFS into file
    FCFS();
    //put result of RR into file
    RR();
    //put result of NPSJF into file
    NPSJF();
    //put result of PSJF into file
    PSJF();


    return 0;
}

void PSJF() {
    fprintf(f,"PSJF:\n");
    int starter = 0;
    int used[50]={};
    int temp = 0;
    int finished = 0;
    int startTime[50] = {};
    int endTime[50]= {};
    int waitTime[50] = {};
    int process[50] = {};
    int remTime[50] = {};
    // store all task index into process
    for (int i = 0; i <size ; i++) {
        process[i] = i;
    }

    // copy the burstTime into remTime
    for (int i = 0; i < size; i++) {
        remTime[i] = burstTime[i];
    }
    int previous = 0;
    int current = 0;
    while(finished!=size){


        // checking the process every 1 ms

        current = process[finished];
        if(current != previous){
            used[previous] = -1;
            fprintf(f,"T%d %d %d\n", previous+1, startTime[previous],endTime[previous]);
            startTime[process[finished]] = starter;
            if(used[process[finished]]==-1){
                waitTime[process[finished]] = starter - endTime[process[finished]];
            }
        }




        endTime[process[finished]] = starter + 1;
        if (remTime[process[finished]] == burstTime[process[finished]]){
            waitTime[process[finished]] = starter-arrivalTime[process[finished]];
        }




        remTime[process[finished]] -=1;
        starter += 1;
        previous = current;

        if (remTime[process[finished]] == 0){
            finished++;
        }


        //go through the for loop to sort the process index by ascending order of burst time
        int smallest = 100;
        for (int i = 0; i <size ; i++) {
            if (remTime[i]<smallest&remTime[i]!=0){
                process[finished] = i;
                smallest = remTime[i];
            }
        }



    }
    // print the last result
    fprintf(f,"T%d %d %d\n", previous+1, startTime[previous],endTime[previous]);


    //print out waiting time
    float totalWait = 0;
    for (int g = 0; g < size; g++){
        fprintf(f,"Waiting time T%d: %d\n",g+1,waitTime[g] );
        totalWait += waitTime[g];
    }


    fprintf(f,"Average Waiting Time: %.2f\n\n",totalWait/size);



}

void NPSJF() {
    fprintf(f,"NPSJF:\n");
    int starter = 0;
    int startTime[50] = {};
    int endTime[50]= {};
    int waitTime[50] = {};
    int process[50] = {};
    int temp = 0;
    int limit = 0;
    //finished is used to monitor the number of tasks that have already finished
    int finished = 0;

    // store all task index into process
    for (int i = 0; i <size ; i++) {
        process[i] = i;
    }

    // print the result of the first arrive
    startTime[0] = starter;
    endTime[0] = burstTime[0] + starter;
    waitTime[0] = starter - arrivalTime[0];
    //put the result into the file
    fprintf(f,"T%d %d %d\n", 1, startTime[0],endTime[0]);
    finished ++;
    starter += burstTime[0];
    // set the limit to burst
    limit = burstTime[0];
    //go through the for loop to sort the process index by ascending order of burst time
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j <size-i-1 ; j++) {
            if (burstTime[j]>burstTime[j+1]){
                temp = process[j+1];
                process[j+1] = process[j];
                process[j] = temp;
            }
        }
    }


    while (finished!=size){
        // go through for loop to pass those arrive late with smaller burst time
        for (int k = 0; k < size; k++) {
            if (arrivalTime[process[k]] <= limit && process[k] != -1 && process[k] !=0){

                startTime[process[k]] = starter;
                endTime[process[k]] = starter + burstTime[process[k]];
                waitTime[process[k]] = starter - arrivalTime[process[k]];
                limit += burstTime[process[process[k]]] ;
                fprintf(f,"T%d %d %d\n", process[k]+1, startTime[process[k]],endTime[process[k]]);
                finished ++;
                starter += burstTime[process[k]];
                // set process that has been executed to -1 to avoid repeated execution
                process[k] = -1;
            }
        }
    }

    //print out waiting time
    float totalWait = 0;
    for (int g = 0; g < size; g++){
        fprintf(f,"Waiting time T%d: %d\n",g+1,waitTime[g] );
        totalWait += waitTime[g];
    }


    fprintf(f,"Average Waiting Time: %.2f\n\n",totalWait/size);




}

void RR() {
    fprintf(f,"RR:\n");
    int starter = 0;
    int startTime[50] = {};
    int endTime[50]= {};
    int waitTime[50] = {};
    int remTime[50] = {};
    //finished is used to monitor the number of tasks that have already finished
    int finished = 0;
    //set count to check if this is first round or other round
    int count = 0;
    // copy the burstTime into remTime
    for (int i = 0; i < size; i++) {
        remTime[i] = burstTime[i];
    }

    // when not all tasks are finished
    while (finished!=size){
        // go through every tasks
        for(int j = 0; j<size;j++){
            // check if the task is completed or not
            if (remTime[j]>0){
                //set quantum to 4
                int quantum = 4;
                // if it is the first round , the waittime will be start time minus arrival time
                if(count == 0){
                    //calculate the wait time
                    waitTime[j] = starter - arrivalTime[j];
                } else{
                    // if this is not first round, the wait time will be current start time minus previous end time
                    waitTime[j] += starter - endTime[j];
                }
                //Add the start time to the list
                startTime[j] = starter;
                //remove the executed time
                remTime[j] -= quantum;
                //if the rest of rem is bigger than 0
                if(remTime[j]>0){
                    //Add end time to the list
                    endTime[j] = starter+quantum;
                }
                // when the rest is less or equal to 0
                else{
                    // deduct quantum to meet the length of execution time
                    quantum += remTime[j];
                    //at this time remTime is 0 or negative number, it can remove the not executed time
                    endTime[j] = starter + remTime[j] +4;

                    finished++;
                }
                //put the result into the file
                fprintf(f,"T%d %d %d\n", j+1, startTime[j],endTime[j]);
                starter+=quantum;
            }
        }
        count++;
    }

    //print out waiting time
    float totalWait = 0;
    for (int g = 0; g < size; g++){
        fprintf(f,"Waiting time T%d: %d\n",g+1,waitTime[g] );
        totalWait += waitTime[g];
    }


    fprintf(f,"Average Waiting Time: %.2f\n\n",totalWait/size);


}

void FCFS(){

    //FCFS algorithm
    //Since the tasks listed in the file are according to thier arrival time.
    //Therefore, there is not much work too do, just simply print out the result
    fprintf(f,"FCFS:\n");
    int starter = 0;
    int startTime[50] = {};
    int endTime[50]= {};
    int waitTime[50] = {};
    for(int i = 0; i<size;i++){
        //Add the start time to the list
        startTime[i] = starter;
        //Add end time to the list
        endTime[i] = starter+burstTime[i];
        //put the result into the file
        fprintf(f,"T%d %d %d\n", i+1, startTime[i],endTime[i]);
        //calculate the wait time
        waitTime[i] = starter - arrivalTime[i];
        //update starter
        starter += burstTime[i];
    }

    //put waittime of all tasks into the file and calculate the avg waiting time
    float totalWait = 0;
    for (int j = 0; j < size; j++){
        fprintf(f,"Waiting time T%d: %d\n",j+1,waitTime[j] );
        totalWait += waitTime[j];
    }


    fprintf(f,"Average Waiting Time: %.2f\n\n",totalWait/size);
}

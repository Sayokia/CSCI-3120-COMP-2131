#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>



/*
 * Some code is reused from A3 description
 */

void *sorter(void *params);
void *merger(void *params);

/* structure for passing data to threads */
typedef struct{
    int starting_index;
    int ending_index;
} parameters;

// initialize two global array to store unsorted and sorted list
int arr[500];
int output[500];


int main(int argc, const char * argv[]) {
    // Read in integer list file
    FILE *f;
    f = fopen("IntegerList.txt","r");

    char str[2000];
    int size = 0;
    int i =0;
    fgets(str,2000,f);
    char *token = strtok(str, ",");
    while (token !=NULL){

        int n = atoi(token);

        arr[i] = n;
        i++;
        token = strtok(NULL, ",");
    }
    fclose(f);
    size = i;


    /* create first worker thread */
    pthread_t tid1;
    parameters *data1 = (parameters *) malloc(sizeof(parameters));
    data1->starting_index= 0;
    data1->ending_index = (size/2)-1;
    pthread_create(&tid1, NULL, sorter, data1);

    /* create second worker thread */
    pthread_t tid2;
    parameters *data2 = (parameters *) malloc(sizeof(parameters));
    data2->starting_index= (size/2);
    data2->ending_index = size-1;
    pthread_create(&tid2, NULL, sorter, data2);

    /* create merge thread */
    pthread_t tid3;
    parameters *data3 = (parameters *) malloc(sizeof(parameters));
    data3->starting_index= 0;
    data3->ending_index = size-1;
    pthread_create(&tid3, NULL, merger, data2);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);


    f = fopen("SortedIntegerList.txt", "w");
    for (int i = 0; i < size; i++) {
        fprintf(f,"%d",output[i]);
        if(i!= size-1){
            fputs(",",f);
        }

    }
    fclose(f);

    return 0;
}

/*
 * The sorter uses Bubble Sort Algorithm
 */
void *sorter(void *params){

    int start = ((parameters *) params)->starting_index;
    int end = ((parameters *) params)->ending_index;

    int temp = 0;

    for (int i = start; i <= end; i++) {
        for (int j = start; j < end ; j++) {
            if (arr[j]>arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }



    pthread_exit(NULL);
}

/*
 * The merge thread
 * Solution is referred to https://www.geeksforgeeks.org/merging-two-unsorted-arrays-sorted-order/
 */
void *merger(void *params){
    int start = ((parameters *) params)->starting_index;
    int end = ((parameters *) params)->ending_index;
    int mid = (end+1)/2;

    int i,j,k;
    // compare the first part with second part and put the smallest into result array
    for ( i = 0, j =mid, k = 0; i<mid &&j<=end; k++) {
        if (arr[i]<arr[j]){
            output[k] = arr[i];
            i++;
        } else{
            output[k] = arr[j];
            j++;
        }
    }

    // put the rest at the back
    while(j<=end){
        output[k] = arr[j];
        j++;
        k++;
    }
    while(i<mid){
        output[k] = arr[i];
        i++;
        k++;
    }

    pthread_exit(NULL);
}



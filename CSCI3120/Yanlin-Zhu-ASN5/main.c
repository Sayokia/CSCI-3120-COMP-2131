#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>



#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE]= {-1,-1,-1,-1,-1};

time_t t;




pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int in,out;

void* producer(void* id);
void insert_item(buffer_item i,int id);

void* consumer(void* id);
int remove_item(int id);

int main(int argc, char*argv[]) {
    int sleepTime, con_num, pro_num;

    in = 0;
    out = 0;





    // get command line arg
    sleepTime = atoi(argv[1]);
    pro_num = atoi(argv[2]);
    con_num = atoi(argv[3]);

    int index_pro[pro_num], index_con[con_num];

    pthread_t pro_tid[pro_num],con_tid[con_num];

    // initialize semaphores and mutex lock
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex,NULL);

    srand((unsigned) time(&t));



    // create producer threads
    for (int i = 0; i < pro_num ; i++) {
        index_pro[i] = i;
        pthread_create(&pro_tid[i],NULL,producer,(void *)&(index_pro[i]));
    }

    //create consumer threads
    for (int j = 0; j < con_num ; j++) {
        index_con[j] = j;
        pthread_create(&con_tid[j],NULL,consumer,(void *)&(index_con[j]));
    }

    //sleep and thereafter terminate c program
    sleep(sleepTime);

    exit(0);
}


void* producer(void* id){
    buffer_item item;
    while(1){
        //sleep random 0-4 s
        int randSleep = rand()%5;
        sleep(randSleep);
        item = rand();
        //insert item
        insert_item(item,*(int*)id);

    }
}

void insert_item(buffer_item i,int id) {
    //make sure there are slots in buffer and decrease empty
    sem_wait(&empty);
    // make sure mutex is not locked
    pthread_mutex_lock(&mutex);

    // insert item at in position
    buffer[in] = i;
    printf("Producer %d inserted item %d into buffer[%d]\n", id , i,in);
    // cirular array
    in =(in+1)%BUFFER_SIZE;

    // unlock metex
    pthread_mutex_unlock(&mutex);
    // increase full
    sem_post(&full);
}

void * consumer(void* id){
    buffer_item item;
    while(1){
        //sleep random 0-4 s

        int randSleep = rand()%5;
        sleep(randSleep);
        item = rand();
        //insert item
        item = remove_item(*(int*)id);
    }
}

int remove_item(int id){
    int item;
    //decrease full
    sem_wait(&full);
    // make sure mutex is not locked
    pthread_mutex_lock(&mutex);

    //get the item
    item = buffer[out];
    printf("Consumer %d consumed item %d from buffer[%d]\n", id, item,out);

    // reset the out position to -1
    buffer[out] = -1;

    out = (out+1)%BUFFER_SIZE;

    // unlock metex
    pthread_mutex_unlock(&mutex);
    // increase empty
    sem_post(&empty);

    return item;
}

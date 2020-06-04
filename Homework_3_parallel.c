#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include "stackHw.h"
#include <pthread.h>

//global varibales
int stackSize, totalNums, count=0; // stack size, total numbers to be pushed, count of numbers pushed
pthread_mutex_t lock; //lock
pthread_cond_t cond; //conditianl varibale

struct Node{
    int value;
    struct Node *next;
    struct Node *prev;
}*tail=NULL;

void push(int value){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
    stackSize++;
    count++; // increment the count of numbers pushed to the stack
}

int pop(){
    struct Node* removed = tail;
    tail = tail->prev;
    tail->next = NULL;
    stackSize--;
    return removed->value;
}

long long getStackCount(){
    return stackSize;
}

// function to handle the threads that are phsing to the stack
void* push_thread(void *arg){
    int *pushes = arg; //total pushes for the thread to perform
    int i;
    //pushes the numbers tot he stack
    for(i=0; i<*pushes; i++){
        pthread_mutex_lock(&lock); //locks the stack
        push(numbers[count%numSize]); //pushes numebrs
        pthread_mutex_unlock(&lock); //unlocks the stack
        pthread_cond_broadcast(&cond); //broadcasts to poping threads that there are elements in the stack
    }
}

// poping thread
void* pop_thread(void *arg){
    //loops until all elements have been pushed and poped from the stack
    while(1){
        pthread_mutex_lock(&lock);//locks the stack
        //if there are no elements in the stack but there are still more pushes to be made
        if(getStackCount() <= 0 && count != totalNums){
            pthread_cond_wait(&cond, &lock); // waits fro signal from a pushing thread that there is now an element in the stack
            pthread_mutex_unlock(&lock); // unlocks the stack
            continue; // continnues the loop
        }
        //thread ends if all numbers have been pushed and poped from the stack
        else if(getStackCount() <= 0 && count == totalNums){
            pthread_mutex_unlock(&lock);
            break;
        }
        else{
            pop();//pops from the stack
            pthread_mutex_unlock(&lock);
        }
    }
}

int main(int argc, char* argv[]){

    if(argc != 3){
        printf("%s: Usage: %s <number od times to push numbers to stack> <thread count>\n", argv[0], argv[0]);
        exit(-1);
    }

    int pushes = atoi(argv[1]);
    int threadCount = atoi(argv[2]); //number of threads
    int i, j;
    pthread_t threads[threadCount-1]; // creates the threads
    GetNumbers("numbers.txt");
    totalNums = pushes * numSize; // toal number of elements to push to the stack
    int numPushThreads;
    //calulates the number of pushes each push threas should perform
    if(threadCount%2 == 0){
        numPushThreads = threadCount/2; //if number of threads is even
    }
    else{
        numPushThreads = (threadCount/2)+1; //if number of threads is odd
    }
    int threadPushes = totalNums/numPushThreads; //number of threads that should perofme pushes
    int pushesLeft = totalNums - (threadPushes*(numPushThreads-1)); //left over number of pushes the last push thread should perform
    tail = (struct Node*)malloc(sizeof(struct Node));

    if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    timing_start();
    //creates the push threads
    for(i=0;i<numPushThreads-1;i++){
        pthread_create(&threads[i], NULL, push_thread, (void *)&threadPushes);
    }
    pthread_create(&threads[i], NULL, push_thread, (void *)&pushesLeft);
    i++;
    //creates the pop threads
    for(i; i<threadCount-1; i++){
        pthread_create(&threads[i], NULL, pop_thread, NULL);
    }
    pop_thread(NULL);

    // joins the threads
    for(i=0;i<threadCount-1; i++){
        pthread_join(threads[i], NULL);
    }
    
    timing_stop();
    print_timing();

    pthread_mutex_destroy(&lock); //destroys the mutex

    return 0;
}
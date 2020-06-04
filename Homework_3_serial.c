#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include "stackHw.h"

long long stackSize; //global stack size varibale

//Node structure for elements in the stack
struct Node{
    int value;//value of element
    struct Node *next; //pointer to next node in the stack
    struct Node *prev; //pointer to the previous node in the stack
}*tail=NULL; //current element on top of the stack

//pushes element onto stack
void push(int value){
    // allocates memory for new node and intializes its fileds
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = value; 
    newNode->prev = tail;
    newNode->next = NULL;
    tail->next = newNode;
    tail = newNode; // new top of stack element is the new node added
    stackSize++; // increases stack size
}

//pops top element from stack
int pop(){
    struct Node* removed = tail;
    tail = tail->prev; // moves top stack pointer to next node down the stack
    tail->next = NULL; // removes element from stack
    stackSize--; //decrements stack size
    return removed->value;
}

//gest the current size of the stack
long long getStackCount(){
    return stackSize;
}

int main(int argc, char* argv[]){
    //makes sure correct number of arguemnts are given
    if(argc != 2){
        printf("%s: Usage: %s <number od times to push numbers to stack>\n", argv[0], argv[0]);
        exit(-1);
    }

    int n = atoi(argv[1]); // sets number of iterations
    GetNumbers("numbers.txt");//gets numbers
    tail = (struct Node*)malloc(sizeof(struct Node));//creates stack

    int i, j; //counters

    timing_start();
    //pushes the numbers to the stack
    for(i=0;i<n;i++){
        for(j=0; j<numSize; j++){
            push(numbers[j]);
        }
    }

    //pops the numbers from the stack
    while(getStackCount() != 0){
        pop();
    }

    timing_stop();
    print_timing();

    return 0;
}
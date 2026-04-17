#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct {
    int* arr;
    int capacity;
    int size;
} PQ;


PQ* createPQ(int capacity){
    PQ* newPQ = (PQ*)malloc(sizeof(PQ));
    newPQ->arr = (int*)malloc(sizeof(int) * capacity);
    newPQ->capacity = capacity;
    newPQ->size = 0;

    return newPQ;
}


void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


bool isEmpty(PQ* pq){
    return pq->size == 0;
}


void heapify(PQ* pq, int idx){
    int left =(2 * idx)+1;
    int right = (2 * idx)+2;
    int minIdx = idx;

    if(left < pq->size && pq->arr[left] < pq->arr[minIdx]){
        minIdx = left;
    }
    if(right < pq->size && pq->arr[right] < pq->arr[minIdx]){
        minIdx = right;
    }

    if(minIdx != idx){
        swap(&pq->arr[minIdx], &pq->arr[idx]);
        heapify(pq, minIdx);
    }
}

void add(PQ* pq, int n){
    if(pq->size >= pq->capacity){
        printf("Priority Queue is full. Cannot add element %d.\n", n);
        return;
    }

    int i = pq->size++;
    pq->arr[i] = n;
    
    while (i != 0 && pq->arr[i] < pq->arr[(i-1)/2])
    {
        swap(&pq->arr[i], &pq->arr[(i-1)/2]);
        i = (i-1)/2;
    }
}

int peek(PQ* pq){
    if(isEmpty(pq)){
        printf("Priority Queue is Empty. Cannot peek elements.\n");
        return -1;
    }

    return pq->arr[0];
}

void pop(PQ* pq){

    if(isEmpty(pq)){
        printf("Priority Queue is Empty. Cannot pop elements.\n");
        return;
    }

    // step 1 :- swap first & last
    swap(&pq->arr[0], &pq->arr[--pq->size]);

    // step 2 :- fix the Priority Queue
    heapify(pq, 0);
}

int main(){
    PQ* pq = createPQ(5);

    add(pq, 5);
    add(pq, 1);
    add(pq, 6);
    add(pq, 2);
    add(pq, 7);

    add(pq, 0);
    while (!isEmpty(pq))
    {
        printf("%d ", peek(pq));
        pop(pq);
    }
}   
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct {
    int* minHeap;
    int* maxHeap;
    int capacity;
    int size;
} DEPQ;

DEPQ* init(int capacity){
    DEPQ* newDEPQ = (DEPQ*)malloc(sizeof(DEPQ));

    if (newDEPQ == NULL) return NULL;

    newDEPQ->capacity = capacity;
    newDEPQ->size = 0;
    newDEPQ->maxHeap = (int*)malloc(sizeof(int) * capacity);
    newDEPQ->minHeap = (int*)malloc(sizeof(int) * capacity);

    if (newDEPQ->maxHeap == NULL || newDEPQ->minHeap == NULL) {
        free(newDEPQ->maxHeap);
        free(newDEPQ->minHeap);
        free(newDEPQ);
        return NULL;
    }

    return newDEPQ;
}

void swap(int* a, int*b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool isEmpty(DEPQ* depq){
    return depq->size == 0;
}

bool isFull(DEPQ* depq) {
    return depq->size == depq->capacity;
}

void heapifyMinUp(DEPQ* depq, int i){
    while(i > 0 && depq->minHeap[i] < depq->minHeap[(i-1)/2]){
        swap(&depq->minHeap[i], &depq->minHeap[(i-1)/2]);
        i = (i - 1) / 2;
    }
}

void heapifyMaxUp(DEPQ* depq, int i){
    while(i > 0 && depq->maxHeap[i] > depq->maxHeap[(i-1)/2]){
        swap(&depq->maxHeap[i], &depq->maxHeap[(i-1)/2]);
        i = (i - 1) / 2;
    }
}

void add(DEPQ* depq, int n){
    if (isFull(depq)) {
        printf("DEPQ is full\n");
        return;
    }

    int i = depq->size++;
    depq->minHeap[i] = n;
    depq->maxHeap[i] = n;

    heapifyMaxUp(depq, i);
    heapifyMinUp(depq, i);
}


int getMin(DEPQ* depq){
    if (isEmpty(depq)) {
        printf("DEPQ is empty\n");
        return -1;
    }
    return depq->minHeap[0];
}


int getMax(DEPQ* depq){
    if (isEmpty(depq)) {
        printf("DEPQ is empty\n");
        return -1;
    }
    return depq->maxHeap[0];
}

void heapifyMinHeap(DEPQ* depq, int idx){
    int left = (2 * idx) + 1;
    int right = (2 * idx) + 2;
    int minIdx = idx;

    if(left < depq->size && depq->minHeap[left] < depq->minHeap[minIdx]){
        minIdx = left;
    } 
    
    if(right < depq->size && depq->minHeap[right] < depq->minHeap[minIdx]){
        minIdx = right;
    }

    if(minIdx != idx){
        swap(&depq->minHeap[minIdx], &depq->minHeap[idx]);
        heapifyMinHeap(depq, minIdx);
    }
}  

void heapifyMaxHeap(DEPQ* depq, int idx){
    int left = (2 * idx) + 1;
    int right = (2 * idx) + 2;
    int maxIdx = idx;

    if(left < depq->size && depq->maxHeap[left] > depq->maxHeap[maxIdx]){
        maxIdx = left;
    } 
    
    if(right < depq->size && depq->maxHeap[right] > depq->maxHeap[maxIdx]){
        maxIdx = right;
    }

    if(maxIdx != idx){
        swap(&depq->maxHeap[maxIdx], &depq->maxHeap[idx]);
        heapifyMaxHeap(depq, maxIdx);
    }
}   


int findInMax(DEPQ* depq, int value){
    for(int i = 0; i< depq->size; i++){
        if(depq->maxHeap[i] == value){
            return i;
        }
    }

    return -1;
}

int findInMin(DEPQ* depq, int value){
    for(int i = 0; i<depq->size; i++){
        if(depq->minHeap[i] == value){
            return i;
        }
    }

    return -1;
}

void deleteMin(DEPQ* depq){
    if(isEmpty(depq)){
        printf("Priority Queue is Empty. Cannot pop elements.\n");
        return;
    }

    int size = --depq->size;
    int value = depq->minHeap[0];
    
    swap(&depq->minHeap[0], &depq->minHeap[size]);
    heapifyMinHeap(depq, 0);

    int idxInMax = findInMax(depq, value);
    if(idxInMax != -1){
        swap(&depq->maxHeap[idxInMax], &depq->maxHeap[size]);
        heapifyMaxUp(depq, idxInMax);
        heapifyMaxHeap(depq, idxInMax);
    }
    return;
}

void deleteMax(DEPQ* depq){
    if(isEmpty(depq)){
        printf("Priority Queue is Empty. Cannot pop elements.\n");
        return;
    }

    int size = --depq->size;
    int value = depq->maxHeap[0];
    
    swap(&depq->maxHeap[0], &depq->maxHeap[size]);
    heapifyMaxHeap(depq, 0);

    int idxInMin = findInMin(depq, value);
    if(idxInMin != -1){
        swap(&depq->minHeap[idxInMin], &depq->minHeap[size]);
        heapifyMinUp(depq, idxInMin);
        heapifyMinHeap(depq, idxInMin);
    }
    return;
}

void freeDEPQ(DEPQ* depq) {
    if (depq == NULL) return;
    free(depq->minHeap);
    free(depq->maxHeap);
    free(depq);
}

void printMinHeap(DEPQ* depq){
    for(int i = 0; i<depq->size; i++){
        printf("%d ", depq->minHeap[i]);
    }
    printf("\n");
}

void printMaxHeap(DEPQ* depq){
    for(int i = 0; i<depq->size; i++){
        printf("%d ", depq->maxHeap[i]);
    }
    printf("\n");
}


int main(){
    DEPQ* depq = init(10);
    if (depq == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    add(depq, 5);
    add(depq, 6);
    add(depq, 2);
    add(depq, 1);
    add(depq, 3);
    add(depq, 7);
    add(depq, 4);

    printf("Min: %d\n", getMin(depq));
    printf("Max: %d\n", getMax(depq));
    printf("MinHeap :- "); printMinHeap(depq);
    printf("MaxHeap :- "); printMaxHeap(depq);

    deleteMin(depq);
    printf("After deleteMin -> Min: %d, Max: %d\n", getMin(depq), getMax(depq));
    
    printf("MinHeap :- "); printMinHeap(depq);
    printf("MaxHeap :- "); printMaxHeap(depq);

    deleteMax(depq);
    printf("After deleteMax -> Min: %d, Max: %d\n", getMin(depq), getMax(depq));
    
    printf("MinHeap :- "); printMinHeap(depq);
    printf("MaxHeap :- "); printMaxHeap(depq);

    freeDEPQ(depq);
    return 0;
}
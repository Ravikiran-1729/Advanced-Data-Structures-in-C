#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


typedef struct Heap{
    int key;
    struct Heap* child;
    struct Heap* sibling;
} Heap;


Heap* init(int key){
    Heap* newHeap = (Heap*)malloc(sizeof(Heap));

    if(newHeap == NULL){
        return NULL;
    }

    newHeap->child = NULL;
    newHeap->key = key;
    newHeap->sibling = NULL;

    return newHeap;
}

bool isEmpty(Heap* heap){
    if(heap == NULL) return true;

    return false;
}


Heap* merge(Heap* h1, Heap* h2){
    if(h1 == NULL) return h2;
    if(h2 == NULL) return h1;

    if(h1->key > h2->key){
        Heap* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    h2->sibling = h1->child;
    h1->child = h2;

    return h1;
}

Heap* insert(Heap* heap, int key){
    Heap* newHeap = init(key);

    return merge(heap, newHeap);
}

Heap* getMin(Heap* heap){
    return heap;
}

Heap* twopassMerge(Heap* list){
    if(list == NULL) return NULL;
    if(list->sibling == NULL) return list;


    Heap* first = list;
    Heap* second = list->sibling;
    Heap* rest = second->sibling;

    first->sibling = NULL;
    second->sibling == NULL;

    return merge(
        merge(first, second), twopassMerge(rest));
}

Heap* deleteMin(Heap* heap){
    if(heap == NULL) return NULL;

    Heap* childList = heap->child;

    free(heap);

    return twopassMerge(childList);
}




int main(){
    Heap* heap = NULL;

    heap = insert(heap, 3);
    heap = insert(heap, 9);
    heap = insert(heap, 7);
    heap = insert(heap, 5);
    heap = insert(heap, 1);
    heap = insert(heap, 2);
    
    while (!isEmpty(heap)){
        printf("Minimum :- %d\n", getMin(heap)->key);
        heap = deleteMin(heap);
    }
}
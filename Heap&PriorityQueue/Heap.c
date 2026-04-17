
#include <stdio.h>
#include <stdlib.h>



typedef struct {
    int* array;     
    int capacity;   
    int size;       
} Heap;

Heap* createHeap(int capacity)
{
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->array = (int*)malloc(capacity * sizeof(int));
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insert(Heap* heap, int value)
{
    if (heap->size == heap->capacity) {
        printf("Heap is full. Cannot insert more elements.\n");
        return;
    }

    int i = heap->size++;
    heap->array[i] = value;
    
    while (i != 0 && heap->array[(i - 1) / 2] < heap->array[i]) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void printHeap(Heap* heap)
{
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

int main()
{
    Heap* heap = createHeap(10);
    insert(heap, 35);
    insert(heap, 33);
    insert(heap, 42);
    insert(heap, 10);
    insert(heap, 14);
    insert(heap, 19);
    insert(heap, 27);
    insert(heap, 44);
    insert(heap, 26);
    insert(heap, 31);
	printf("Heap elements :- ");
    printHeap(heap);

    return 0;
}
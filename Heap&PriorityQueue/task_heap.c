
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int task_id;
    int time;
    int priority;
} Task;

typedef struct {
    Task* *array;     
    int capacity;   
    int size;       
} Heap;

Task* createTask(int task_id, int time, int priority){
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->task_id = task_id;
    newTask->time = time;
    newTask->priority = priority;
    return newTask;
}

Heap* createHeap(int capacity)
{
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->array = (Task**)malloc(capacity * sizeof(Task*));
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void swap(Task** a, Task** b)
{
    Task* temp = *a;
    *a = *b;
    *b = temp;
}

void insert(Heap* heap, int task_id, int time, int priority)
{
    if (heap->size == heap->capacity) {
        printf("Heap is full. Cannot insert more elements.\n");
        return;
    }


    int i = heap->size++;
    heap->array[i] = createTask(task_id, time, priority);
    
    while (i != 0 && (heap->array[(i - 1) / 2])->priority < (heap->array[i])->priority) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void printHeap(Heap* heap)
{
    
	printf("Task id :- ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]->task_id);
    }
    printf("\nTask Priority :- ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]->priority);
    }
    printf("\n");
}

int main()
{
    Heap* heap = createHeap(10);
    insert(heap, 1, 12, 35);
    insert(heap, 2, 52, 33);
    insert(heap, 3, 45, 42);
    insert(heap, 4, 100, 10);
    insert(heap, 5, 10, 144);
    insert(heap, 6, 56, 19);
    insert(heap, 7, 29, 27);
    insert(heap, 8, 17, 44);
    insert(heap, 9, 25, 26);
    insert(heap, 10, 9, 31);


    printHeap(heap);

    return 0;
}
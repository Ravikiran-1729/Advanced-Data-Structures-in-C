#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <limits.h>

typedef struct Node
{
    int key;
    int degree;
    struct Node* parent;
    struct Node* child;
    struct Node* sibling;
}Node;

Node* createNode(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->key = key;
    newNode->child = NULL;
    newNode->parent = NULL;
    newNode->sibling = NULL;
    newNode->degree = 0;

    return newNode;
}


Node* reverseList(Node* head){
    if(head == NULL || head->sibling == NULL){
        return head;
    }

    Node* newNode = reverseList(head->sibling);

    head->sibling->sibling = head;
    head->sibling = NULL;

    return newNode;
}


Node* findMin(Node* head){
    Node* y = NULL;
    Node* x = head;

    int min = INT_MAX;


    while (x != NULL)
    {
        if(x->key < min){
            min = x->key;
            y = x;
        }
        x = x->sibling;
    }
    
    return y;
}


Node* linkTree(Node* y, Node* z){
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;

    return z;
}


Node* unionHeap(Node* head){
    if (head == NULL) return NULL;

    Node* prev = NULL;
    Node* curr = head;
    Node* next = curr->sibling;

    while (next != NULL){
        if(curr->degree != next->degree || 
            (next->sibling != NULL && next->sibling->degree == curr->degree)){
                prev = curr;
                curr = next;
        }else{
            if(curr->key <= next->key){
                curr->sibling = next->sibling;

                linkTree(next, curr);
            }else{
                if(prev == NULL){
                    head = next;
                }else{
                    prev->sibling = next;
                }

                linkTree(curr, next);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    
    return head;
}

Node* merge(Node* H1, Node* H2){
    if (H1 == NULL) return H2;
    if (H2 == NULL) return H1;

    Node* head = NULL; 
    Node* tail = NULL;
    
    Node* i = H1;
    Node* j = H2;

    if(i->degree <= j->degree){
        head = i;
        i = i->sibling;
    }else{
        head = j;
        j = j->sibling;
    }

    tail = head;

    while (i != NULL && j != NULL)
    {
        if(i->degree <= j->degree){
            tail->sibling = i;
            i = i->sibling;
        }else{
            tail->sibling = j;
            j = j->sibling;
        }
        tail = tail->sibling;
    }
    

    if(i != NULL){
        tail->sibling = i;
    }

    if(j != NULL){
        tail->sibling = j;
    }

    return unionHeap(head);
}




Node* insert(Node* head, int key){
    Node* newNode = createNode(key);

    if(head == NULL) return newNode;

    return merge(head, newNode);
}


Node* deleteMin(Node* head){
    if(head == NULL) return NULL;

    Node* minNode = findMin(head);

    Node* prev = NULL;
    Node* curr = head;

    while (curr != NULL && curr != minNode)
    {
        prev = curr;
        curr = curr->sibling;
    }

    if(prev == NULL){
        head = minNode->sibling;
    }else{
        prev->sibling = minNode->sibling;
    }

    Node* childHeap = reverseList(minNode->child);


    // reset parent pointers of reversed child list
    Node* temp = childHeap;
    while(temp != NULL){
        temp->parent = NULL;
        temp = temp->sibling;
    }
    
    free(minNode);

    return merge(head, childHeap);
}


void printRootList(Node* head) {
    while (head != NULL) {
        printf("%d(deg=%d) ", head->key, head->degree);
        head = head->sibling;
    }
    printf("\n");
}


int main() {
    Node* head = NULL;

    int arr[] = {2, 1, 5, 3, 8, 4};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Inserting elements into Binomial Heap:\n");
    for (int i = 0; i < n; i++) {
        head = insert(head, arr[i]);
        printf("After inserting %d: ", arr[i]);
        printRootList(head);

        Node* minNode = findMin(head);
        if (minNode != NULL) {
            printf("Current Min = %d\n", minNode->key);
        }
        printf("\n");
    }

    printf("Final Root List: ");
    printRootList(head);

    Node* minNode = findMin(head);
    if (minNode != NULL) {
        printf("Final Min = %d\n", minNode->key);
    }

    printf("\nDeleting minimum...\n");
    head = deleteMin(head);

    printf("Root List after deleteMin: ");
    printRootList(head);

    minNode = findMin(head);
    if (minNode != NULL) {
        printf("New Min = %d\n", minNode->key);
    } else {
        printf("Heap is empty\n");
    }

    return 0;
}
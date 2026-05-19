#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>



typedef struct Node{
    int key;
    struct Node* left;
    struct Node* right;
} Node;


Node* init(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->key = key;
    newNode->left =  newNode->right = NULL;

    return newNode;
}


void swap(Node** a, Node** b){
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

Node* merge(Node* h1, Node* h2){
    if(h1 == NULL) return h2;
    if(h2 == NULL) return h1;


    if(h1->key > h2->key){
        swap(&h1, &h2);
    }

    
    h1->right = merge(h1->right, h2);

    swap(&h1->left, &h1->right);

    return h1;
}



Node* insert(Node* heap, int key){
    Node* newNode = init(key);

    return merge(heap, newNode);
}


Node* deleteMin(Node* heap){
    if(heap == NULL) return NULL;
    
    return merge(heap->left, heap->right);
}

Node* findMin(Node* heap){
    return heap;
}

void preorder(Node* root) {
    if (root == NULL) return;

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

int main() {
    Node* heap = NULL;

    heap = insert(heap, 10);
    heap = insert(heap, 5);
    heap = insert(heap, 2);
    heap = insert(heap, 8);
    heap = insert(heap, 15);
    heap = insert(heap, 3);


    Node* minNode = findMin(heap);
    if (minNode != NULL) {
        printf("Min = %d\n", minNode->key);
    }

    printf("Preorder: ");
    preorder(heap);
    printf("\n");

    heap = deleteMin(heap);

    minNode = findMin(heap);
    if (minNode != NULL) {
        printf("Min after delete = %d\n", minNode->key);
    }

    printf("Preorder after delete: ");
    preorder(heap);
    printf("\n");

    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


// npl - null path length 
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int npl;
} Node;


Node* init(int data){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL){ return NULL;}

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->npl = 0;

    return newNode;
}

int getNPL(Node* root){
    if(root == NULL) return -1;

    return root->npl;
}

Node* merge(Node* h1, Node* h2){
    if(h1 == NULL) return h2;
    if(h2 == NULL) return h1;

    if(h1->data > h2->data){
        Node* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    h1->right = merge(h1->right, h2);

    if(getNPL(h1->left) < getNPL(h1->right)){
        Node* temp = h1->right;
        h1->right = h1->left;
        h1->left = temp;
    }

    h1->npl = getNPL(h1->right) + 1;

    return h1;
}

Node* insert(Node* root, int data){
    Node* newNode = init(data);

    return merge(root, newNode);
}

Node* deleteMin(Node* root){
    if(root == NULL) return NULL;

    Node* left = root->left;
    Node* right = root->right;

    free(root);

    return merge(left, right);
}

void preOrder(Node* root){
    if(root == NULL) return;

    printf("%d ", root->data);
    preOrder(root->left);
    preOrder(root->right);
}


int main(){

    Node* root = NULL;

    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 8);
    root = insert(root, 15);

    printf("Min = %d\n", root->data);
    preOrder(root);
    printf("\n");
    
    
    root = deleteMin(root);
    
    printf("Min after delete = %d\n", root->data);
    preOrder(root);
    printf("\n");

}
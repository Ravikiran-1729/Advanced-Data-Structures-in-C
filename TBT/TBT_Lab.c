#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int value;
    int lchild, rchild;
    struct Node* left, *right;
} Node;


Node* createNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(!newNode){
        printf("Unable to create New Node");
        return NULL;
    }

    newNode->value = value;
    newNode->lchild = 1;
    newNode->rchild = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node* insert(int value, Node* root){
    Node* newNode = createNode(value);
    if(root == NULL){
        return newNode;
    }

    Node* queue[100];
    int front = 0, rare = 0;

    queue[rare++] = root;

    while (front < rare)
    {
        Node* curr = queue[front++];

        if(curr->lchild == 1 || curr->left == NULL){
            curr->lchild = 0;
            newNode->left = curr->left;
            curr->left = newNode;
            newNode->right = curr;
            return root;
        }else{
            queue[rare++] = curr->left;
        }
        if(curr->rchild == 1 || curr->right == NULL){
            curr->rchild = 0;
            newNode->right = curr->right;
            curr->right = newNode;
            newNode->left = curr;
            return root;
        }else{
            queue[rare++] = curr->right;
        }
    }
    return root;
    
}

Node* inorderSuccessor(Node* curr){
    if(curr->rchild == 1){
        return curr->right;
    }

    curr = curr->right;
    while (curr->lchild == 0)
    {
        curr = curr->left;
    }

    return curr;
}

void inorder(Node* root){

    if (root == NULL){
        return;
    }
    
    Node* curr = root;
    while (curr->lchild == 0){
        curr = curr->left;
    }

    while (curr != NULL){
        printf("%d ", curr->value);
        curr = inorderSuccessor(curr);
    }
}

int main(){
    Node* root = NULL;

    // root = insert(5, root);
    // root = insert(8, root);
    // root = insert(7, root);
    // root = insert(2, root);
    // root = insert(9, root);
    // root = insert(1, root);

    // inorder(root);

    int choice, val;

    while (1) {
        printf("\n--- Threaded Binary Tree ---\n");
        printf("1. Insert\n");
        printf("2. Inorder Traversal\n");
        printf("3. Preorder Traversal\n");
        printf("4. Search Node\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                root = insert(val, root);
                break;

            case 2:
                inorder(root);
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}
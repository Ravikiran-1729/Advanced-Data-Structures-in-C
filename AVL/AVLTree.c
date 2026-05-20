#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct Node{
    int data, height;
    struct Node* left, *right;
} Node;


Node* createNode(int data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(!newNode){
        return NULL;
    }
    newNode->data = data;
    newNode->height = 1;
    newNode->left = newNode->right = NULL;

    return newNode;
}

int getHeight(Node* root){
    if(!root){
        return 0;
    }
    return 1 + max(getHeight(root->left), getHeight(root->right));
}

int getBalanceFactor(Node* root){
    if(!root){
        return 0;
    }
    return getHeight(root->left) - getHeight(root->right);
}

Node* leftRotation(Node* root){
    Node* child = root->right;
    Node* childLeft = child->left;

    child->left = root;
    root->right = childLeft;

    return child;
}
Node* rightRotation(Node* root){
    Node* child = root->left;
    Node* childRight = child->right;

    child->right = root;
    root->left = childRight;

    return child;
}

Node* insert(Node* root,int data){
    if(!root){
        return createNode(data);
    }

    if(root->data > data){
        root->left = insert(root->left,data);
    }else if(root->data < data){
        root->right = insert(root->right, data);
    }else{
        return root;
    }

    root->height = getHeight(root);

    int balancefactor = getBalanceFactor(root);

    // LL rotation
    if(balancefactor > 1 && root->left->data > data){
        return rightRotation(root);
    }
    // RR rotation
    else if(balancefactor < -1 && root->right->data < data){
        return leftRotation(root);
    }
    // LR rotation
    else if(balancefactor > 1 && root->left->data < data){
        root->left =  leftRotation(root->left);
        return rightRotation(root);
    }
    // RL rotation
    else if(balancefactor < -1 && root->right->data > data){
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }else{
        return root;
    }
}



void inorder(Node* root){
    if(!root){
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}




int main(){
    Node* root = NULL;
    
    root = insert(root, 69);
    root = insert(root, 39);
    root = insert(root, 35);
    root = insert(root, 37);
    root = insert(root, 20);
    root = insert(root, 19);
    root = insert(root, 59);
    root = insert(root, 49);

    printf("root :- %d\n", root->data);
    printf("root :- %d\n", root->right->data);
    printf("root :- %d\n", root->right->right->data);
    inorder(root);
}
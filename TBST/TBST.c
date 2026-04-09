#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Node{
    int key;
    struct Node* left;
    struct Node* right;
    int lthread; // 1 : thread, 0 : child
    int rthread; // 1 : thread, 0 : child
} Node;

Node* createNode(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->lthread = newNode->rthread = 1;

    return newNode;
}

Node* leftMostNode(Node* root){
    if(root == NULL) return NULL;
    Node* curr = root;
    while (curr->lthread == 0)
    {
        curr = curr->left;
    }   
    return curr;
}

Node* RightMostNode(Node* root){
    if(root == NULL) return NULL;
    Node* curr = root;
    while (curr->rthread == 0)
    {
        curr = curr->right;
    }   
    return curr;
}

Node* inorderSuccessor(Node* node){
    if(node == NULL) return NULL;
    
    if(node->rthread == 1){
        return node->right;
    }
    
    return leftMostNode(node->right);
}

Node* inorderPredecessor(Node* node){
    if(node == NULL) return NULL;
    
    if(node->lthread == 1){
        return node->left;
    }
    
    return RightMostNode(node->left);
}


Node* insert(Node* root, int key, Node* DUMMY){
    Node* newNode = createNode(key);
    if(newNode == NULL) return NULL;

    if(root == NULL){
        newNode->left = DUMMY;
        newNode->right = DUMMY;
        DUMMY->left = newNode;
        return newNode; 
    }

    Node* curr = root;

    while (curr != DUMMY )
    {
        if(curr->key > key){
            if(curr->lthread == 0){
                curr = curr->left;
                continue;
            }   

            newNode->left = curr->left;
            newNode->right = curr;

            curr->left = newNode;
            curr->lthread = 0;
            return root;
        }else if(curr->key < key){
            if(curr->rthread == 0){
                curr = curr->right;
                continue;
            }
            newNode->right = curr->right;
            newNode->left = curr;

            curr->right = newNode;
            curr->rthread = 0;
            return root;
        }else{
            free(newNode);
            return root;
        }
    }
    return root;
}


Node* delete(Node* root, int key, Node* DUMMY){
    Node* parent = NULL;
    Node* curr = root;
    
    while (curr != DUMMY)
    {
        if(key == curr->key){
            break;
        }

        parent = curr;
        
        if(curr->key > key){
            if(curr->lthread == 0){
                curr = curr->left;
            }else{
                return root;
            }
        }else{
            if(curr->rthread == 0){
                curr = curr->right;
            }else{
                return root;
            }
        }
    }
    
    
    if(curr == NULL) return root;
    
    if(curr->lthread == 0 && curr->rthread == 0){
        Node* succ = inorderSuccessor(curr);
        int val = succ->key;
        root = delete(root, val, DUMMY);
        curr->key = val;
        return root;
    }
    
    else if(curr->lthread == 1 && curr->rthread == 1){
        if(parent == NULL){
            root = NULL;
            DUMMY->left = DUMMY;
        }else if(parent->left == curr){
            parent->left = curr->left;
            parent->lthread = 1;
        }else{
            parent->right = curr->right;
            parent->rthread = 1;
        }
        
        free(curr);
    }else{
        Node* child;
        
        if(curr->lthread == 0){
            child = curr->left;
        }else {
            child = curr->right;
        }
        
        if(parent == NULL){
            root = child;
            DUMMY->left = root;
        }else if(parent->left == curr){
            parent->left = child;
        }else{
            parent->right = child;
        }
        
        Node* pred = inorderPredecessor(curr);
        Node* succ = inorderSuccessor(curr);
        
        
        if(curr->lthread == 0){
            pred->right = succ;
        }else if(curr->rthread == 0){
            succ->left = pred;
        }
        
        free(curr);
    }
    return root;
}


void inorder(Node* root, Node* DUMMY){
    if(root == NULL){
        printf("Tree is empty\n");
        return;
    }

    Node* curr = leftMostNode(root);

    while (curr != DUMMY)
    {
        printf("%d ", curr->key);

        if(curr->rthread == 1){
            curr = curr->right;
        }else{
            curr = leftMostNode(curr->right);
        }
    }
    printf("\n");
}

int main(){
    Node* DUMMY = createNode(-1);
    DUMMY->left = DUMMY;
    DUMMY->right = DUMMY;
    
    Node* root = NULL;
    
    root = insert(root, 5, DUMMY);
    root = insert(root, 9, DUMMY);
    root = insert(root, 1, DUMMY);
    root = insert(root, 0, DUMMY);
    root = insert(root, 6, DUMMY);
    root = insert(root, 4, DUMMY);

    inorder(root, DUMMY);

    root = delete(root,5, DUMMY);

    inorder(root, DUMMY);
}
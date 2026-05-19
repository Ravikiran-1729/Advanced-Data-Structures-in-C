#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define ALPHABETS 26

typedef struct Node{
    struct Node* *children;
    bool eow;
} Node;


Node* createNode(){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->children = (Node**)malloc(sizeof(Node*) * ALPHABETS);

    if(newNode->children == NULL){
        free(newNode);
        return NULL;
    }

    newNode->eow = false;
    
    for(int i = 0; i<ALPHABETS; i++){
        newNode->children[i] = NULL;
    }

    return newNode;
}

int calculateIdx(char ch){
    return ch - 'a';
}

Node* insert(Node* root, char* prefix){

    Node* curr = root;

    for(int i = 0; prefix[i] != '\0'; i++){
        int idx = calculateIdx(prefix[i]);

        if(curr->children[idx] == NULL){
            curr->children[idx] = createNode();
        }
        curr = curr->children[idx];
    }
    curr->eow = true;
    return root;
}


bool search(Node* root, char* prefix){
    Node* curr = root;

    for(int i = 0; prefix[i] != '\0'; i++){
        int idx = calculateIdx(prefix[i]);

        if(curr->children[idx] == NULL){
            return false;
        }
        curr = curr->children[idx];
    }

    if(!curr->eow) return false;

    return true;
}


bool startWith(Node* root, char* prefix){
    Node* curr = root;

    for(int i = 0; prefix[i] != '\0'; i++){
        int idx = calculateIdx(prefix[i]);

        if(curr->children[idx] == NULL){
            return false;
        }
        curr = curr->children[idx];
    }

    return true;
}


int countNodes(Node* root){
    if(root == NULL){
        return 0;
    }

    int count = 0;
    for(int i = 0; i<ALPHABETS; i++){
        count += countNodes(root->children[i]);
    }

    return count + 1;
}


int main() {
    Node* root = createNode();

    char* arr[] = {"the", "a", "apple", "app", "there", "their"};
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, arr[i]);
    }

    printf("startWith(\"appl\")  = %s\n", startWith(root, "app")? "TRUE" : "FALSE");
    printf("search(\"appl\")  = %s\n", search(root, "appl")? "TRUE" : "FALSE");
    printf("search(\"apple\") = %s\n", search(root, "apple")? "TRUE" : "FALSE");
    printf("search(\"app\")   = %s\n", search(root, "app")? "TRUE" : "FALSE");
    printf("search(\"the\")   = %s\n", search(root, "the")? "TRUE" : "FALSE");
    printf("search(\"their\") = %s\n", search(root, "their")? "TRUE" : "FALSE");
    printf("search(\"bat\")   = %s\n", search(root, "bat")? "TRUE" : "FALSE");

    printf("Count of Nodes = %d\n", countNodes(root));
    return 0;
}

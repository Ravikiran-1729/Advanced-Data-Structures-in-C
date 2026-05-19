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


char* getPrefix(char* string, int len){
    char* firstPart = (char*)malloc(sizeof(char) * (len + 1));
    if(firstPart == NULL) return NULL;

    strncpy(firstPart, string, len);
    firstPart[len] = '\0';

    return firstPart;
}


bool wordBreak(Node* root, char* string){
    if(string[0] == '\0') return true;

    for(int i = 0; string[i] != '\0'; i++){
        char* firstPart = getPrefix(string, i + 1);
        if(firstPart == NULL) return false;

        char* secondPart = string + i + 1;

        if(search(root, firstPart) && wordBreak(root, secondPart)){
            printf("First Part = %s\n", firstPart);
            free(firstPart);
            return true;
        }

        free(firstPart);
    }
    return false;
}

int main() {
    Node* root = createNode();

    char* words[] = {"i", "like", "sam", "samsung", "mobiles"};

    int n = sizeof(words) / sizeof(words[0]);

    for(int i = 0; i<n; i++){
        root = insert(root, words[i]);
    }

    char* str = "ilikesamsung";

    printf("%d\n", wordBreak(root, str));


    return 0;
}
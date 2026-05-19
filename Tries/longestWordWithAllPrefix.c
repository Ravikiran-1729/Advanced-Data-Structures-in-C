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



void getLongest(Node* root, char*  temp, char* longest){
    if(root == NULL) return;

    for(int i = 0; i<ALPHABETS; i++){
        if(root->children[i] != NULL && root->children[i]->eow == true){
            int tempLen = strlen(temp);

            temp[tempLen] = 'a' + i;
            temp[tempLen+1] = '\0';
            

            if(strlen(temp) > strlen(longest)){
                strcpy(longest, temp);
            }

            getLongest(root->children[i], temp, longest);

            temp[tempLen] = '\0';
        }
    }
}

int main() {
    Node* root = createNode();

    char* words[] = {"a", "ap", "app", "appl", "apple", "apply","applye","applyee", "b", "ba", "ban", "bana", "banane", "banana", "banan"};

    int n = sizeof(words) / sizeof(words[0]);

    for(int i = 0; i<n; i++){
        root = insert(root, words[i]);
    }

    char* temp = (char*)malloc(sizeof(char) * 1000);
    char* longest = (char*)malloc(sizeof(char) * 1000);

    temp[0] ='\0';
    longest[0] ='\0';
    
    getLongest(root, temp, longest);

    printf("\n%s\n", longest);

    return 0;
}
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


int countUniqueSubstrings(Node* root, char* word){
    for(int i = 0; word[i] != '\0'; i++){
        root = insert(root, word+i);
    }

    /*
    In Trie method,
    each node represents one substring.

    Root represents empty string.

    So,
    unique substrings = countNodes(root) - 1
    */
    return countNodes(root) - 1;
}

int main() {
    Node* subRoot = createNode();
    char word[] = "banana";

    printf("Unique substrings of \"%s\" = %d\n", word, countUniqueSubstrings(subRoot, word));

    return 0;
}
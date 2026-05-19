
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


typedef struct Node{
    char* label;
    bool eow;
    struct Node* child;
    struct Node* sibling;
} Node;


Node* createNode(char*  label, bool eow){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->label = (char*)malloc(strlen(label)+1);

    if(newNode->label == NULL){
        free(newNode);
        return NULL;
    }

    strcpy(newNode->label ,label);
    newNode->eow = eow;
    newNode->child = NULL;
    newNode->sibling = NULL;

    return newNode;
}



int lcp(char* a, char* b){
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i])
    {
        i++;
    }
    return i;
}

char* makeSubstring(char* word, int start, int len){
    char* subString = (char*)malloc(len + 1);

    for (int i = 0; i < len; i++)
    {
        subString[i] = word[start + i];
    }
    subString[len] = '\0';
    
    return subString;
}

void insert(Node* root, char* word){
    if(word[0] == '\0') return;
    
    Node* prev = NULL;
    Node* curr = root->child;


    while(curr != NULL){
        
        int longestCommonPrefix = lcp(curr->label, word);
        
        // 1. No Match
        if(longestCommonPrefix == 0){
            prev = curr;
            curr = curr->sibling;
            continue;
        }

        int wordLen = strlen(word);
        int labelLen = strlen(curr->label);

        // 2. Full Match len(label) == len(longestCommonPrefix)
        if(labelLen == longestCommonPrefix){
            if(labelLen == wordLen){
                curr->eow = true;
            }else{
                char* remaining = makeSubstring(word, longestCommonPrefix, wordLen - longestCommonPrefix);
                insert(curr, remaining);
                free(remaining);
            }

            return;
        }

        // 3. Partially Match len(label) > len(longestCommonPrefix)
        char* oldSuffix = makeSubstring(curr->label, longestCommonPrefix, labelLen-longestCommonPrefix);
        char* newSuffix = makeSubstring(word, longestCommonPrefix, wordLen-longestCommonPrefix);
        char* commonPrefix = makeSubstring(curr->label, 0, longestCommonPrefix);

        Node* oldChild = curr->child;
        bool oldEow = curr->eow;

        free(curr->label);

        curr->label = commonPrefix;
        curr->eow = (longestCommonPrefix == wordLen);

        Node* oldSuffixNode = createNode(oldSuffix, oldEow);
        free(oldSuffix);

        oldSuffixNode->child = oldChild;
        curr->child = oldSuffixNode;

        if (longestCommonPrefix < wordLen) {
            Node* newSuffixNode = createNode(newSuffix, true);
            free(newSuffix);

            if (newSuffixNode == NULL) return;
            oldSuffixNode->sibling = newSuffixNode;
        }else {
            free(newSuffix);
        }
        return;

    }

    Node* newNode = createNode(word, true);
    if (newNode == NULL) return;

    if (prev == NULL) {
        root->child = newNode;
    } else {
        prev->sibling = newNode;
    }
}


bool search(Node* root, char* word){
    Node* curr = root->child;


    while (curr != NULL)
    {
        int common = lcp(curr->label, word);
        int wordLen = strlen(word);
        int labelLen = strlen(curr->label);

        if(common == 0){
            curr = curr->sibling;
            continue;
        }

        
        if(common == labelLen && common == wordLen ){
            return curr->eow;
        }

        if(common == labelLen){
            word = word + common;
            curr = curr->child;
        }else{
            return false;
        }
    }

    return false;
}

void printTrie(Node* root, int level) {
    if (root == NULL) return;

    Node* curr = root->child;
    while (curr != NULL) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("%s%s\n", curr->label, curr->eow ? "*" : "");
        printTrie(curr, level + 1);
        curr = curr->sibling;
    }
}

int main() {
    Node* root = createNode("", false);

    insert(root, "there");
    insert(root, "their");
    insert(root, "the");
    insert(root, "then");
    insert(root, "apple");
    insert(root, "app");

    printTrie(root, 0);
    bool exists = search(root, "ahe");

    printf("%d\n", exists);

    return 0;
}
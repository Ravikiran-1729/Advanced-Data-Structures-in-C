#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef struct Node{
    int start;              // inclusive
    int end;                // exclusive
    struct Node* child;
    struct Node* sibling;
} Node;


Node* createNode(int start, int end){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->start = start;
    newNode->end = end;
    newNode->child = NULL;
    newNode->sibling = NULL;

    return newNode;
}


int lcp(char* s, int i, int j){
    int k = 0;

    while (s[i + k] && s[j + k] && s[i + k] == s[j + k]){
        k++;
    }
    
    return k;
}


int lcpEdge(char* s, Node*  node, int start){
    int i = node->start;
    int j = start;
    int k = 0;

    while (i < node->end && s[j] && s[i] == s[j])
    {
        i++;
        j++;
        k++;
    }

    return k;
}

Node* insert(Node* root, char* word, int start){
    if(root == NULL){
        root = createNode(-1,-1);
        if(root == NULL) return NULL;
    }

    Node* prev = NULL;
    Node* curr = root->child;
    int wordLen = (int)strlen(word);

    while (curr != NULL)
    {
        int longestCommonPrefix = lcpEdge(word, curr, start);

        if(longestCommonPrefix == 0){
            prev = curr;
            curr = curr->sibling;
            continue;
        }

        int labelLen = curr->end - curr->start;
        int  suffixLen = wordLen - start;

        // Full edge Matched
        if(labelLen == longestCommonPrefix){
            if(suffixLen == longestCommonPrefix){
                return root;    // exact suffix already present
            }
            insert(curr, word, start + longestCommonPrefix);
            return root;
        }


        // Partial match -> split current edge
        Node* oldChild = curr->child;

        int oldStart = curr->start;
        int splitPoint = oldStart + longestCommonPrefix;
        
        // Old suffix branch
        Node* oldSuffixNode = createNode(splitPoint, curr->end);
        if(oldSuffixNode == NULL) return root;
        oldSuffixNode->child = oldChild;
        
        curr->end = splitPoint;
        curr->child = oldSuffixNode;
        
        if(longestCommonPrefix < suffixLen){
            Node* newSuffixNode = createNode(start + longestCommonPrefix, wordLen);
            if(newSuffixNode == NULL) return root;
            oldSuffixNode->sibling = newSuffixNode;
        }
        return root;
    }

    Node*  newSuffixNode = createNode(start, wordLen);
    if (newSuffixNode == NULL) return root;

    if(prev == NULL){
        root->child = newSuffixNode;
    }else{
        prev->sibling = newSuffixNode;
    }
    return root;
}



void printLabel(const char* s, int start, int end) {
    for (int i = start; i < end; i++) {
        putchar(s[i]);
    }
}

void printTree(Node* root, const char* s, int level) {
    if (root == NULL) return;
    

    Node* curr = root->child;
    while (curr != NULL) {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printLabel(s, curr->start, curr->end);
        printf(" [%d,%d)\n", curr->start, curr->end);

        printTree(curr, s, level + 1);
        curr = curr->sibling;
    }
}


int main() {
    char word[] = "mississippi$";
    int n = strlen(word);

    Node* root = createNode(-1, -1);
    if (root == NULL) {
        printf("Memory allocation failed for root\n");
        return 1;
    }


    // Insertion into Suffix Tree
    for (int i = 0; i < n; i++) {
        root = insert(root, word, i);
    }
    

    // Display Suffix Tree
    printf("====================================\n");
    printf("String        : %s\n", word);
    printf("Length        : %d\n", n);
    printf("====================================\n\n");

    printf("Suffix Tree:\n\n");
    printTree(root, word, 0);

    return 0;
}
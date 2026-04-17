#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


char* text;

typedef struct Node{
    int start;
    int end;
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

int lcp(char* str, int i, int j){
    int k = 0;
    while (str[i+k] && str[j+k] && str[i+k] == str[j+k])
    {
        k++;
    }

    return k;
}

int lcpEdge(Node* node, char* str, int start){
    int i = node->start;
    int j = start;
    int k = 0;
    while (i < node->end && str[j] && str[i] == str[j])
    {
        k++;
        i++;
        j++;
    }

    return k;
}


Node* insert(Node* root, char* suffix, int start){
    if(root == NULL){
        root = createNode(-1,-1);
        if(root == NULL) return NULL;
    }

    Node* prev = NULL;
    Node* curr = root->child;
    int wordLen = (int)strlen(suffix);

    while (curr != NULL)
    {
        int common = lcpEdge(curr, suffix, start);

        if(common == 0){
            prev = curr;
            curr = curr->sibling;
            continue;
        }

        int labelLen = curr->end - curr->start;
        int suffixLen = wordLen - start;

        if(common == labelLen){
            if(common = suffixLen){
                return root;
            }
            root = insert(curr, suffix, start + common);
            return root;
        }

        Node* oldChild = curr->child;
        int oldStart = curr->start;
        int splitPoint = oldStart + common;

        Node* oldSuffixNode = createNode(splitPoint, curr->end);
        if(oldSuffixNode == NULL) return root;
        oldSuffixNode->child = oldChild;

        curr->end = splitPoint;
        curr->child = oldSuffixNode;

        if(common < suffixLen){
            Node* newNode = createNode(start + common, wordLen);
            if(newNode == NULL) return root;
            oldSuffixNode->sibling = newNode;
        }

        return root;
    }
    
    Node* newNode = createNode(start, wordLen);

    if(prev == NULL){
        root->child = newNode;
    }else{
        prev->sibling = newNode;
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


bool search(Node* root, char* text, char* pattern){
    if(root == NULL | root->child == NULL) return false;

    Node* curr = root;
    int i = 0;
    while (i < strlen(pattern)){
        
        Node* child = curr->child;

        while (child != NULL)
        {
            if(text[child->start] == pattern[i]){
                break;
            }
            child = child->sibling;
        }

        if(child == NULL){
            return false;
        }

        int k = child->start;
        while (k < child->end && i < strlen(pattern))
        {
            if(text[k] != pattern[i]){
                return false;
            }
            i++;
            k++;
        }
        curr = child;
    }
    return true;
}


int cmp(const void* a, const void* b){
    int i = *(int*)a;
    int j = *(int*)b;

    return strcmp(text+i, text+j);
}

void getSuffixArray(char* string){
    int n = strlen(string);

    int* SA = (int*)malloc(sizeof(int) * n);

    if(SA == NULL) return;

    for(int i = 0; i<n; i++){
        SA[i] = i;
    }

    text = string;
    qsort(SA, n, sizeof(int), cmp);


    printf("\n====================================\n");
    printf("Suffix Array Indices:\n");
    printf("====================================\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", SA[i]);
    }

    printf("\n\n====================================\n");
    printf("Sorted Suffixes:\n");
    printf("====================================\n");
    for (int i = 0; i < n; i++) {
        printf("[%2d] index = %2d   suffix = %s\n", i, SA[i], string + SA[i]);
    }

    free(SA);
}



int main(){
    Node* root = NULL;

    char string[] = "mississippi$";


    // Insertion into Suffix Tree
    for(int i = 0; string[i] != '\0'; i++){
        root = insert(root, string, i);
    }


    // Display Suffix Tree
    printf("====================================\n");
    printf("String        : %s\n", string);
    printf("Length        : %d\n", strlen(string));
    printf("====================================\n\n");

    printf("Suffix Tree:\n\n");
    printTree(root, string, 0);



    // Pattern Matching
    char* pattern = "ana$";
    printf("\n\nIs Pattern '%s' exists : %s\n\n", pattern, search(root, string, pattern) ? "true" : "false");

    
    // Suffix Array
    getSuffixArray(string);
}

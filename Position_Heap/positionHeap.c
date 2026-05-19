#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


typedef struct Node{
    char c;
    int pos;
    struct Node* child;
    struct Node* sibling;
} Node;


Node* createNode(char c){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->c = c;
    newNode->child = NULL;
    newNode->sibling = NULL;
    newNode->pos = -1;

    return newNode;
}

Node* findChild(Node* curr, char ch){
    if(curr == NULL) return NULL;

    Node* child = curr->child;

    while (child != NULL)
    {
        if(child->c == ch){
            return child;
        }

        child = child->sibling;
    }
    
    return child;
}

Node* getLastNode(Node* curr){
    if(curr == NULL) return NULL;

    Node* lastNode = NULL;
    Node* child = curr->child;

    while (child != NULL)
    {
        lastNode = child;
        child = child->sibling;
    }

    return lastNode;
}

Node* insert(Node* root, char* suffix, int pos){
    Node* curr = root;

    for(int i = 0; i<strlen(suffix); i++){
        Node* child = findChild(curr, suffix[i]);

        if(child != NULL){
            curr = child;
        }else{
            Node* newNode = createNode(suffix[i]);
            Node* lastNode = getLastNode(curr);

            if(lastNode == NULL){
                curr->child = newNode;
            }else{
                lastNode->sibling = newNode;
            }
            curr = newNode;
        }
    }
    curr->pos = pos;

    return root;
}


void printTree(Node* root, int level) {
    if (root == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("%c", root->c);
    if (root->pos != -1) {
        printf(" (%d)", root->pos);
    }
    printf("\n");

    printTree(root->child, level + 1);
    printTree(root->sibling, level);
}


int main(){
    Node* root = createNode('$');

    char str[] = "mississippi";

    for (int i = 0; str[i] != '\0'; i++) {
        printf("%s\n",str+i);
        root = insert(root, str + i, i + 1);
    }
    
    printf("\nPosition Heap:\n");
    printTree(root, 0);


    printf("\nFirst child of root = %c\n", root->child->c);
    printf("%s", str);
}
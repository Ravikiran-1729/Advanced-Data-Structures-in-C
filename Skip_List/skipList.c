#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define MAX_LEVEL 3
#define P 0.5

typedef struct  Node
{
    int key;
    struct Node* *forward;
}Node;

typedef struct SkipList
{
    int level;
    Node* head;
}SkipList;


Node* createNode(int key, int level){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) return NULL;

    newNode->key = key;
    newNode->forward = (Node**)malloc((level + 1) * sizeof(Node*));

    for(int i = 0; i<=level; i++){
        newNode->forward[i] = NULL;
    }

    return newNode;
}


SkipList* createSkipList(){
    SkipList* List = (SkipList*)malloc(sizeof(SkipList));
    if(List == NULL) return NULL;
    
    List->level = 0;
    List->head = createNode(-1, MAX_LEVEL);
    
    return List;
}

int randomLevel() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < P && level < MAX_LEVEL) {
        level++;
    }
    return level;
}
void insert(SkipList* list, int key){
    Node* update[MAX_LEVEL+1];
    Node* curr = list->head;

    for(int i = list->level; i>= 0; i--){
        while(curr->forward[i] != NULL && curr->forward[i]->key < key){
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];

    if(curr != NULL && curr->key == key){
        return;
    }

    int newLevel = randomLevel();

    if(newLevel > list->level){
        for(int i = list->level + 1; i<= newLevel; i++){
            update[i] = list->head;
        }
        list->level = newLevel;
    }

    Node* newNode = createNode(key, newLevel);

    for(int i = 0; i <= newLevel; i++){
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

Node* search(SkipList* list, int key){
    Node* curr = list->head;

    for(int i = list->level; i>= 0; i--){
        while (curr->forward[i] != NULL && curr->forward[i]->key < key){
                curr = curr->forward[i];
        }
    }
    

    curr = curr->forward[0];

    if(curr != NULL && curr->key == key){
        return curr;
    }
    return NULL;
}



void deleteNode(SkipList* list, int key){

    Node* curr = list->head;
    Node* update[MAX_LEVEL+1];

    for(int i = list->level; i>=0; i--){
        while (curr->forward[i] != NULL && curr->forward[i]->key < key)
        {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];

    if(curr != NULL && curr->key == key){
        for(int i = 0; i<=list->level; i++){
            if(update[i]->forward[i] != curr){
                break;
            }
            update[i]->forward[i] = curr->forward[i];
        }

        free(curr->forward);
        free(curr);

        while (list->level > 0 && list->head->forward[list->level] == NULL)
        {
            list->level--;
        }
    }
}



void printSkipList(SkipList* list) {
    for (int i = list->level; i >= 0; i--) {
        Node* curr = list->head->forward[i];
        printf("Level %d: ", i);
        while (curr != NULL) {
            printf("%d ", curr->key);
            curr = curr->forward[i];
        }
        printf("\n");
    }
}


int main(){
    srand(time(NULL));
    SkipList* list = createSkipList();

    
    insert(list, 6);
    insert(list, 17);
    insert(list, 7);
    insert(list, 3);
    insert(list, 9);
    insert(list, 21);
    insert(list, 25);
    insert(list, 26);
    insert(list, 12);
    insert(list, 19);
    
    printSkipList(list);
    Node* n = search(list, 9);

    if(n == NULL) {
        printf("NULL");
    }else{
        printf("%d\n", n->key);
        deleteNode(list, n->key);
        printSkipList(list);
    }

    return 0;
}
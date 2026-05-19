#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int low;
    int high;
    int max;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int low, int high){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) return NULL;

    newNode->low = low;
    newNode->high = high;
    newNode->max = high;
    newNode->left = newNode->right = NULL;

    return newNode;
}

int modifyMax(Node* node){
    int max = node->max;

    if(node->left && node->left->max > max){
        max = node->left->max;
    }

    if(node->right && node->right->max > max){
        max = node->right->max;
    }
    return max;
}

Node* insert(Node* root, int low, int high){
    if(root == NULL){
        return createNode(low, high);
    }

    if(root->low >= low){
        root->left = insert(root->left, low, high);
    }else{
        root->right = insert(root->right, low, high);
    }

    root->max = modifyMax(root);
    return root;
}

void findOverlapsAll(Node* root){
    if(root == NULL){
        return;
    }

    if(root->left && root->low < root->left->high && root->left->low < root->high){
        printf("[%d, %d] & [%d, %d]\n", root->low, root->high, root->left->low, root->left->high);  
    }
    if(root->right && root->low < root->right->high && root->right->low < root->high){
        printf("[%d, %d] & [%d, %d]\n", root->low, root->high, root->right->low, root->right->high);  
    }


    findOverlapsAll(root->left);
    findOverlapsAll(root->right);
    return;
}


void findOverlaps(Node* root, int low, int high){
    if(root == NULL) return;

    if(root->low < high && low < root->high){
        printf("[%d, %d]\n", root->low, root->high);  
    }

    findOverlaps(root->left, low, high);
    findOverlaps(root->right, low, high);
}


Node* findLeftMostNode(Node* node){
    if(node == NULL){
        return NULL;
    }
    Node* curr = node;

    while (curr->left != NULL)
    {
        curr = curr->left;
    }
    return curr;
}



Node* delete(Node* root, int low, int high){
    if(root == NULL) return NULL;

    if(root->low > low){
        root->left = delete(root->left, low, high);
    }else if(root->low < low){
        root->right = delete(root->right, low, high);
    }else{
        if(root->right == NULL){
            Node* temp = root->left;
            free(root);
            return temp;
        }else if(root->left == NULL){
            Node* temp = root->right;
            free(root);
            return temp;
        }

        Node* succ = findLeftMostNode(root->right);

        if(succ == NULL) return NULL;

        root->low = succ->low;
        root->high = succ->high;

        root->right = delete(root->right, succ->low, succ->high);
    }
    
    root->max = modifyMax(root);
    return root;
}

void preorder(Node* root){
    if(root == NULL){
        return;
    }

    printf("Low = %d; High = %d; Max = %d\n", root->low, root->high, root->max);

    preorder(root->left);
    preorder(root->right);
}

int main(){
    Node* root = NULL;
    int choice;
    int low, high;

    root = insert(root, 15, 20); 
    root = insert(root, 10, 30); 
    root = insert(root, 17, 19); 
    root = insert(root, 5, 20); 
    root = insert(root, 12, 15); 
    root = insert(root, 30, 40); 
    root = insert(root, 16, 20);

    while (1) {
        printf("\n===== Interval Tree Menu =====\n");
        printf("1. Insert Interval\n");
        printf("2. Delete Interval\n");
        printf("3. Preorder Traversal\n");
        printf("4. Find Overlapping Intervals\n");
        printf("5. Find All Overlapping Intervals\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                printf("Enter interval (low high): ");
                scanf("%d %d", &low, &high);
                root = insert(root, low, high);
                printf("Inserted [%d, %d]\n", low, high);
                break;

            case 2:
                printf("Enter interval to delete (low high): ");
                scanf("%d %d", &low, &high);
                root = delete(root, low, high);
                printf("Deleted [%d, %d] (if existed)\n", low, high);
                break;

            case 3:
                printf("Preorder Traversal:\n");
                preorder(root);
                printf("\n");
                break;

            case 4:
                printf("Enter interval to find overlaps (low high): ");
                scanf("%d %d", &low, &high);
                printf("Overlapping intervals:\n");
                findOverlaps(root, low, high);
                printf("\n");
                break;
            case 5:
                printf("All Overlapping intervals:\n");
                findOverlapsAll(root);
                printf("\n");
                break;

            case 6:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
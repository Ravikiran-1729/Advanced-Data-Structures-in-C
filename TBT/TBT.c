#include <stdio.h>
#include <stdlib.h>

struct TBT {
    int data;
    struct TBT *left, *right;
    int lthread, rthread;
};

struct TBT *root = NULL;

/* Create a new node */
struct TBT* createNode(int data) {
    struct TBT *temp = (struct TBT*)malloc(sizeof(struct TBT));
    temp->data = data;
    temp->left = temp->right = NULL;
    temp->lthread = 1;
    temp->rthread = 1;
    return temp;
}

/* Insert node into Threaded Binary Tree */
void insert(int data) {
    struct TBT *ptr = root;
    struct TBT *parent = NULL;

    while (ptr != NULL) {
        if (data == ptr->data) {
            printf("Duplicate value not allowed\n");
            return;
        }

        parent = ptr;

        if (data < ptr->data) {
            if (ptr->lthread == 0)
                ptr = ptr->left;
            else
                break;
        } else {
            if (ptr->rthread == 0)
                ptr = ptr->right;
            else
                break;
        }
    }

    struct TBT *newNode = createNode(data);

    if (parent == NULL) {
        root = newNode;
    }
    else if (data < parent->data) {
        newNode->left = parent->left;
        newNode->right = parent;
        parent->lthread = 0;
        parent->left = newNode;
    }
    else {
        newNode->left = parent;
        newNode->right = parent->right;
        parent->rthread = 0;
        parent->right = newNode;
    }
}

/* Find inorder successor */
struct TBT* inorderSuccessor(struct TBT *ptr) {
    if (ptr->rthread == 1)
        return ptr->right;

    ptr = ptr->right;
    while (ptr->lthread == 0)
        ptr = ptr->left;

    return ptr;
}

/* Inorder traversal */
void inorder() {
    struct TBT *ptr = root;

    if (ptr == NULL) {
        printf("Tree is empty\n");
        return;
    }

    while (ptr->lthread == 0)
        ptr = ptr->left;

    printf("Inorder Traversal: ");
    while (ptr != NULL) {
        printf("%d ", ptr->data);
        ptr = inorderSuccessor(ptr);
    }
    printf("\n");
}

/* Preorder traversal */
void preorder() {
    struct TBT *ptr = root;

    if (ptr == NULL) {
        printf("Tree is empty\n");
        return;
    }

    printf("Preorder Traversal: ");
    while (ptr != NULL) {
        printf("%d ", ptr->data);

        if (ptr->lthread == 0)
            ptr = ptr->left;
        else {
            while (ptr != NULL && ptr->rthread == 1)
                ptr = ptr->right;
            if (ptr != NULL)
                ptr = ptr->right;
        }
    }
    printf("\n");
}

/* Search a node */
void search(int key) {
    struct TBT *ptr = root;

    while (ptr != NULL) {
        if (key == ptr->data) {
            printf("Node %d found\n", key);
            return;
        }

        if (key < ptr->data) {
            if (ptr->lthread == 0)
                ptr = ptr->left;
            else
                break;
        } else {
            if (ptr->rthread == 0)
                ptr = ptr->right;
            else
                break;
        }
    }

    printf("Node %d not found\n", key);
}

/* Main Menu */
int main() {
    int choice, val;

    while (1) {
        printf("\n--- Threaded Binary Tree ---\n");
        printf("1. Insert\n");
        printf("2. Inorder Traversal\n");
        printf("3. Preorder Traversal\n");
        printf("4. Search Node\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                insert(val);
                break;

            case 2:
                inorder();
                break;

            case 3:
                preorder();
                break;

            case 4:
                printf("Enter value to search: ");
                scanf("%d", &val);
                search(val);
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}
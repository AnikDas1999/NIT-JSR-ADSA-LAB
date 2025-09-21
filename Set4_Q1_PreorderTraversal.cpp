#include <stdio.h>
#include <stdlib.h>

// Define structure for tree node
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

// Preorder traversal without recursion
void preorderTraversal(struct Node* root) {
    if (root == NULL) return;

    // Create a stack
    struct Node* stack[100];
    int top = -1;

    // Push root to stack
    stack[++top] = root;

    // Iterate until stack is empty
    while (top >= 0) {
        // Pop from stack and print
        struct Node* node = stack[top--];
        printf("%d ", node->data);

        // Push right child first so that left is processed first
        if (node->right)
            stack[++top] = node->right;
        if (node->left)
            stack[++top] = node->left;
    }
}

// Driver code

int main() {
    
    struct Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->right = createNode(6);

    printf("Preorder traversal: ");
    preorderTraversal(root);

    return 0;
}































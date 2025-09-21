// avl.c
#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int key;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

/* Utility */
int max(int a, int b) { return (a > b) ? a : b; }
int height(AVLNode *n) { return n ? n->height : 0; }

AVLNode* createNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

/* Rotations */
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode* n) {
    return n ? (height(n->left) - height(n->right)) : 0;
}

/* createTree(): return NULL root (empty tree) */
AVLNode* createTree() {
    return NULL;
}

/* insertitem(root, key) */
AVLNode* insertitem(AVLNode* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insertitem(node->left, key);
    else if (key > node->key)
        node->right = insertitem(node->right, key);
    else // equal keys not allowed
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    // Right Right
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    // Left Right
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

/* serachltem(root, key) */
AVLNode* serachltem(AVLNode* root, int key) {
    if (root == NULL) return NULL;
    if (key == root->key) return root;
    if (key < root->key) return serachltem(root->left, key);
    return serachltem(root->right, key);
}

/* minValueNode */
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

/* deleteltem(root, key) */
AVLNode* deleteltem(AVLNode* root, int key) {
    if (root == NULL) return root;

    if (key < root->key)
        root->left = deleteltem(root->left, key);
    else if (key > root->key)
        root->right = deleteltem(root->right, key);
    else {
        // node with one or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                // no child
                temp = root;
                root = NULL;
            } else {
                // one child
                *root = *temp; // copy
            }
            free(temp);
        } else {
            // node with two children
            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteltem(root->right, temp->key);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Balance cases
    // Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    // Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    // Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

/* deleteTree(root) */
void deleteTree(AVLNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

/* helper: inorder print */
void preorderPrint(AVLNode* root) {
    if (!root) return;
    printf("%d ", root->key);
    preorderPrint(root->left);
    preorderPrint(root->right);
}

/* Demo main */
int main() {
    AVLNode* root = createTree();

    int keys[] = { 10, 20, 30, 40, 50, 25 };
    int n = sizeof(keys)/sizeof(keys[0]);
    for (int i = 0; i < n; ++i)
        root = insertitem(root, keys[i]);

    printf("Preorder after inserts: ");
    preorderPrint(root);
    printf("\n");

    root = deleteltem(root, 40);
    printf("Preorder after deleting 40: ");
    preorderPrint(root);
    printf("\n");

    AVLNode* found = serachltem(root, 25);
    printf("Search 25: %s\n", found ? "FOUND" : "NOT FOUND");

    deleteTree(root);
    return 0;
}


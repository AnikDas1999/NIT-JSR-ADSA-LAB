// btree.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct BTreeNode {
    int *keys;          // keys
    int t;              // min degree
    struct BTreeNode **C; // children
    int n;              // current number of keys
    bool leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode *root;
    int t;
} BTree;

/* Helper create node */
BTreeNode* bnodeCreate(int t, bool leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int*)malloc(sizeof(int)*(2*t-1));
    node->C = (BTreeNode**)malloc(sizeof(BTreeNode*)*(2*t));
    node->n = 0;
    for (int i=0;i<2*t;i++) node->C[i] = NULL;
    return node;
}

BTree* createTree(int t) {
    BTree* T = (BTree*)malloc(sizeof(BTree));
    T->t = t;
    T->root = bnodeCreate(t, true);
    return T;
}

/* search */
BTreeNode* serachltem_node(BTreeNode* x, int k) {
    int i = 0;
    while (i < x->n && k > x->keys[i]) i++;
    if (i < x->n && x->keys[i] == k) return x;
    if (x->leaf) return NULL;
    return serachltem_node(x->C[i], k);
}
BTreeNode* serachltem(BTree* T, int k) {
    if (!T->root) return NULL;
    return serachltem_node(T->root, k);
}

/* split child y of x at index i */
void btreeSplitChild(BTreeNode* x, int i, BTreeNode* y) {
    int t = y->t;
    BTreeNode* z = bnodeCreate(t, y->leaf);
    z->n = t-1;
    for (int j=0;j<t-1;j++) z->keys[j] = y->keys[j+t];
    if (!y->leaf) {
        for (int j=0;j<t;j++) z->C[j] = y->C[j+t];
    }
    y->n = t-1;
    for (int j = x->n; j >= i+1; j--) x->C[j+1] = x->C[j];
    x->C[i+1] = z;
    for (int j = x->n-1; j >= i; j--) x->keys[j+1] = x->keys[j];
    x->keys[i] = y->keys[t-1];
    x->n = x->n + 1;
}

/* insert non-full */
void btreeInsertNonFull(BTreeNode* x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i+1] = x->keys[i];
            i--;
        }
        x->keys[i+1] = k;
        x->n += 1;
    } else {
        while (i >= 0 && x->keys[i] > k) i--;
        i++;
        if (x->C[i]->n == 2*x->t - 1) {
            btreeSplitChild(x, i, x->C[i]);
            if (x->keys[i] < k) i++;
        }
        btreeInsertNonFull(x->C[i], k);
    }
}

/* insertitem(tree, key) */
void insertitem(BTree* T, int k) {
    BTreeNode* r = T->root;
    if (r->n == 2*T->t - 1) {
        BTreeNode* s = bnodeCreate(T->t, false);
        T->root = s;
        s->C[0] = r;
        btreeSplitChild(s, 0, r);
        btreeInsertNonFull(s, k);
    } else btreeInsertNonFull(r, k);
}

/* deleteltem: TODO (complex) */
void deleteltem(BTree* T, int k) {
    // Full B-tree deletion requires many cases (borrow/merge). It's long to implement.
    // Placeholder: search and if found, remove only if leaf (shift keys); otherwise print not implemented.
    BTreeNode* n = serachltem(T, k);
    if (!n) return;
    if (n->leaf) {
        int idx = 0;
        while (idx < n->n && n->keys[idx] != k) idx++;
        if (idx == n->n) return;
        for (int j=idx;j<n->n-1;j++) n->keys[j] = n->keys[j+1];
        n->n--;
    } else {
        fprintf(stderr, "deleteltem: full B-tree deletion not implemented for internal nodes (key=%d)\n", k);
    }
}

/* deleteTree: recursive free */
void freeNode(BTreeNode* x) {
    if (!x) return;
    if (!x->leaf) {
        for (int i=0;i<=x->n;i++) freeNode(x->C[i]);
    }
    free(x->keys);
    free(x->C);
    free(x);
}
void deleteTree(BTree* T) {
    if (!T) return;
    freeNode(T->root);
    free(T);
}

/* Demo print (simple) */
void traverse(BTreeNode* x) {
    int i;
    for (i=0;i<x->n;i++) {
        if (!x->leaf) traverse(x->C[i]);
        printf("%d ", x->keys[i]);
    }
    if (!x->leaf) traverse(x->C[i]);
}

/* Demo main */
int main() {
    BTree* T = createTree(3); // t=3
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i=0;i<8;i++) insertitem(T, keys[i]);
    printf("B-tree traversal: ");
    traverse(T->root);
    printf("\n");

    BTreeNode* s = serachltem(T, 6);
    printf("Search 6: %s\n", s ? "FOUND" : "NOT FOUND");

    deleteltem(T, 6);
    printf("After deleteltem(6) traversal: ");
    traverse(T->root);
    printf("\n");

    deleteTree(T);
    return 0;
}


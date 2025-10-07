/*
 * dfs_edge_classify.c
 *
 * Directed-graph DFS that classifies edges into:
 *   TREE, BACK, FORWARD, CROSS
 *
 * Compile:
 *   gcc -std=c11 -O2 dfs_edge_classify.c -o dfs
 *
 * Run (interactive input):
 *   ./dfs
 * Input format (example):
 *   n m
 *   u1 v1
 *   u2 v2
 *   ...
 * where vertices are 0..n-1
 *
 * If you enter n=0, program runs a built-in demo graph.
 */

#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY  1
#define BLACK 2

typedef struct {
    int n;
    int *cap;
    int *sz;
    int **adj;
} Graph;

Graph *create_graph(int n) {
    Graph *g = malloc(sizeof(Graph));
    if (!g) { perror("malloc"); exit(1); }
    g->n = n;
    g->cap = calloc(n, sizeof(int));
    g->sz  = calloc(n, sizeof(int));
    g->adj = calloc(n, sizeof(int*));
    if (!g->cap || !g->sz || !g->adj) { perror("calloc"); exit(1); }
    for (int i = 0; i < n; ++i) {
        g->cap[i] = 4;
        g->sz[i] = 0;
        g->adj[i] = malloc(sizeof(int) * g->cap[i]);
        if (!g->adj[i]) { perror("malloc"); exit(1); }
    }
    return g;
}

void add_edge(Graph *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return;
    if (g->sz[u] == g->cap[u]) {
        g->cap[u] *= 2;
        int *tmp = realloc(g->adj[u], sizeof(int) * g->cap[u]);
        if (!tmp) { perror("realloc"); exit(1); }
        g->adj[u] = tmp;
    }
    g->adj[u][g->sz[u]++] = v;
}

void free_graph(Graph *g) {
    if (!g) return;
    for (int i = 0; i < g->n; ++i) free(g->adj[i]);
    free(g->adj);
    free(g->cap);
    free(g->sz);
    free(g);
}

/* Globals used during DFS */
int *color = NULL, *dtime = NULL, *ftime = NULL, timer = 0;
Graph *G = NULL;

/* Classify and print one directed edge (u -> v).
 * Must be called *when edge (u,v) is explored*.
 */
void print_edge_classification(int u, int v) {
    if (color[v] == WHITE) {
        printf("(%d -> %d) : TREE\n", u, v);
    } else if (color[v] == GRAY) {
        printf("(%d -> %d) : BACK\n", u, v);
    } else { /* BLACK */
        /* If discovery(u) < discovery(v) then (u,v) goes to descendant => FORWARD
           else it's to a previously finished unrelated subtree => CROSS */
        if (dtime[u] < dtime[v])
            printf("(%d -> %d) : FORWARD\n", u, v);
        else
            printf("(%d -> %d) : CROSS\n", u, v);
    }
}

void dfs_visit(int u) {
    color[u] = GRAY;
    dtime[u] = ++timer;
    for (int i = 0; i < G->sz[u]; ++i) {
        int v = G->adj[u][i];
        if (color[v] == WHITE) {
            /* Edge to undiscovered vertex -> TREE */
            print_edge_classification(u, v); /* shows TREE */
            dfs_visit(v);
        } else {
            /* v is GRAY or BLACK -> classify accordingly */
            print_edge_classification(u, v);
        }
    }
    color[u] = BLACK;
    ftime[u] = ++timer;
}

void dfs_all() {
    timer = 0;
    color = malloc(sizeof(int) * G->n);
    dtime = malloc(sizeof(int) * G->n);
    ftime = malloc(sizeof(int) * G->n);
    if (!color || !dtime || !ftime) { perror("malloc"); exit(1); }

    for (int i = 0; i < G->n; ++i) {
        color[i] = WHITE;
        dtime[i] = ftime[i] = 0;
    }

    for (int u = 0; u < G->n; ++u)
        if (color[u] == WHITE)
            dfs_visit(u);
}

int main(void) {
    int n, m;
    printf("Enter number of vertices n and edges m (enter 0 for n to use demo): ");
    if (scanf("%d", &n) != 1) return 0;

    if (n == 0) {
        /* Demo graph (directed) that illustrates each edge type */
        n = 8;
        G = create_graph(n);
        add_edge(G, 0, 1);
        add_edge(G, 0, 2);
        add_edge(G, 1, 2);
        add_edge(G, 2, 0); /* back edge forming cycle */
        add_edge(G, 2, 3);
        add_edge(G, 3, 3); /* self-loop -> back edge */

        add_edge(G, 4, 5);
        add_edge(G, 5, 6);
        add_edge(G, 6, 4); /* back edge in component */
        add_edge(G, 6, 7);
    } else {
        printf("Enter number of edges m: ");
        if (scanf("%d", &m) != 1) { free_graph(G); return 0; }
        G = create_graph(n);
        printf("Now enter %d directed edges (u v) with 0 <= u,v < %d\n", m, n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            if (scanf("%d %d", &u, &v) != 2) { printf("Invalid input\n"); free_graph(G); return 0; }
            add_edge(G, u, v);
        }
    }

    printf("\nEdge classifications (format: (u -> v) : TYPE )\n\n");
    dfs_all();

    printf("\nDiscovery/Finish times:\n");
    for (int i = 0; i < G->n; ++i)
        printf("vertex %d : d = %d, f = %d\n", i, dtime[i], ftime[i]);

    /* cleanup */
    free(color);
    free(dtime);
    free(ftime);
    free_graph(G);
    return 0;
}


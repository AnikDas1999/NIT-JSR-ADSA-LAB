#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

#define MAXN 100005    // maximum number of nodes (adjust as needed)
#define MAXM 200005    // maximum number of edges (adjust as needed)

typedef struct {
    int u, v;
    ll w;
} Edge;

Edge edges[MAXM];
int edge_count = 0;

/* Disjoint Set Union (Union-Find) */
int parent[MAXN];
int rankv[MAXN];

void dsu_init(int n) {
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
        rankv[i] = 0;
    }
}

int dsu_find(int x) {
    if (parent[x] != x) parent[x] = dsu_find(parent[x]);
    return parent[x];
}

int dsu_union(int a, int b) {
    a = dsu_find(a);
    b = dsu_find(b);
    if (a == b) return 0;
    if (rankv[a] < rankv[b]) parent[a] = b;
    else if (rankv[b] < rankv[a]) parent[b] = a;
    else { parent[b] = a; rankv[a]++; }
    return 1;
}

/* qsort comparator for edges by weight (ascending) */
int cmp_edge(const void *p1, const void *p2) {
    Edge *a = (Edge*)p1;
    Edge *b = (Edge*)p2;
    if (a->w < b->w) return -1;
    if (a->w > b->w) return 1;
    return 0;
}

/* Kruskal's algorithm */
void kruskal(int n, int m) {
    qsort(edges, m, sizeof(Edge), cmp_edge);
    dsu_init(n);

    ll total_w = 0;
    int taken = 0;

    printf("MST edges (u - v : w):\n");
    for (int i = 0; i < m && taken < n - 1; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        ll w = edges[i].w;
        if (dsu_find(u) != dsu_find(v)) {
            dsu_union(u, v);
            printf("  %d - %d : %lld\n", u, v, w);
            total_w += w;
            taken++;
        }
    }

    /* Check connectivity / forest info */
    int components = 0;
    for (int i = 1; i <= n; ++i) if (dsu_find(i) == i) components++;

    if (components == 1) {
        printf("\nTotal weight of MST = %lld\n", total_w);
    } else {
        printf("\nGraph is disconnected. Minimum spanning forest contains %d edges (across %d components).\n", taken, components);
        printf("Total weight of the forest = %lld\n", total_w);
    }
}

int main() {
    int n, m;
    printf("Enter n (nodes) and m (edges):\n");
    if (scanf("%d %d", &n, &m) != 2) return 0;
    if (n <= 0) return 0;
    if (m < 0 || m >= MAXM) return 0;

    printf("Enter %d edges (u v w) - undirected edges:\n", m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        if (scanf("%d %d %lld", &u, &v, &w) != 3) {
            fprintf(stderr, "Invalid input on edge %d\n", i+1);
            return 0;
        }
        if (u < 1 || u > n || v < 1 || v > n) {
            fprintf(stderr, "Edge endpoints out of range: %d %d\n", u, v);
            return 0;
        }
        edges[i].u = u;
        edges[i].v = v;
        edges[i].w = w;
    }
    edge_count = m;

    kruskal(n, m);
    return 0;
}


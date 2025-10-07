#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;
const ll INF = (1LL<<60);

typedef struct Edge {
    int to;
    ll weight;
    int next;
} Edge;

#define MAXN 100005    // adjust max nodes as needed
#define MAXM 200005    // adjust max edges (undirected stored twice) as needed

Edge edges[MAXM];
int head[MAXN];
int edge_cnt = 0;

void add_edge_directed(int u, int v, ll w) {
    edges[edge_cnt].to = v;
    edges[edge_cnt].weight = w;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt++;
}

/* helper to add undirected edge (both directions) */
void add_edge(int u, int v, ll w) {
    add_edge_directed(u, v, w);
    add_edge_directed(v, u, w);
}

/* Min-heap entries: (weight, node, parent) */
ll heap_w[MAXM];
int heap_node[MAXM];
int heap_parent[MAXM];
int heap_size = 0;

void heap_swap(int i, int j) {
    ll tw = heap_w[i]; heap_w[i] = heap_w[j]; heap_w[j] = tw;
    int tn = heap_node[i]; heap_node[i] = heap_node[j]; heap_node[j] = tn;
    int tp = heap_parent[i]; heap_parent[i] = heap_parent[j]; heap_parent[j] = tp;
}

void heap_push(ll w, int node, int parent) {
    int i = ++heap_size;
    heap_w[i] = w;
    heap_node[i] = node;
    heap_parent[i] = parent;
    while (i > 1)  {
        int p = i >> 1;
        if (heap_w[p] <= heap_w[i]) break;
        heap_swap(p, i);
        i = p;
    }
}

int heap_pop(ll *out_w, int *out_node, int *out_parent) {
    if (heap_size == 0) return 0;
    *out_w = heap_w[1];
    *out_node = heap_node[1];
    *out_parent = heap_parent[1];
    heap_swap(1, heap_size--);
    int i = 1;
    while (1) {
        int l = i << 1, r = l | 1, smallest = i;
        if (l <= heap_size && heap_w[l] < heap_w[smallest]) smallest = l;
        if (r <= heap_size && heap_w[r] < heap_w[smallest]) smallest = r;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
    return 1;
}

/* Prim's algorithm: builds MST starting from node 'start' */
void prim(int n, int start) {
    int visited[MAXN] = {0};
    ll total_weight = 0;
    int mst_edges = 0;

    heap_size = 0;
    // push start with parent = -1 and weight 0
    heap_push(0, start, -1);

    printf("MST edges (u - v : w):\n");
    while (heap_size) {
        ll w; int u; int parent;
        heap_pop(&w, &u, &parent);
        if (visited[u]) continue;
        visited[u] = 1;
        if (parent != -1) {
            printf("  %d - %d : %lld\n", parent, u, w);
            total_weight += w;
            ++mst_edges;
        }
        // add neighbors
        for (int ei = head[u]; ei != -1; ei = edges[ei].next) {
            int v = edges[ei].to;
            ll ew = edges[ei].weight;
            if (!visited[v]) {
                heap_push(ew, v, u);
            }
        }
    }

    // Check if all nodes visited (graph connected)
    int cnt = 0;
    for (int i = 1; i <= n; ++i) if (visited[i]) ++cnt;
    if (cnt != n) {
        printf("\nGraph is disconnected — Prim produced a minimum spanning forest.\n");
        printf("Visited %d out of %d nodes; MST contains %d edges (sum over visited components = %lld).\n",
               cnt, n, mst_edges, total_weight);
    } else {
        printf("\nTotal weight of MST = %lld\n", total_weight);
    }
}

int main() {
    int n, m;
    printf("Enter n (nodes) and m (edges):\n");
    if (scanf("%d %d", &n, &m) != 2) return 0;
    if (n <= 0) return 0;

    // init heads
    for (int i = 1; i <= n; ++i) head[i] = -1;
    edge_cnt = 0;

    printf("Enter %d undirected edges (u v w) one per line:\n", m);
    for (int i = 0; i < m; ++i) {
        int u, v; ll w;
        scanf("%d %d %lld", &u, &v, &w);
        if (u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge endpoints: %d %d\n", u, v);
            return 0;
        }
        add_edge(u, v, w);
    }

    int start = 1;
    printf("Enter start node for Prim (1..%d) [or 0 for default 1]:\n", n);
    if (scanf("%d", &start) != 1) start = 1;
    if (start == 0) start = 1;
    if (start < 1 || start > n) start = 1;

    prim(n, start);
    return 0;
}


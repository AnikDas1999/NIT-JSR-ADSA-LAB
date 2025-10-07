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

#define MAXN 100005    // max nodes (adjust as needed)
#define MAXM 200005    // max edges (adjust as needed)
Edge edges[MAXM];
int head[MAXN];
int edge_cnt = 0;

void add_edge(int u, int v, ll w) {
    edges[edge_cnt].to = v;
    edges[edge_cnt].weight = w;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt++;
}

/* Min-heap of (dist, node) with lazy deletions */
ll heap_dist[MAXM];    // heap values (distance)
int heap_node[MAXM];
int heap_size = 0;

void heap_swap(int i, int j) {
    ll td = heap_dist[i]; heap_dist[i] = heap_dist[j]; heap_dist[j] = td;
    int tn = heap_node[i]; heap_node[i] = heap_node[j]; heap_node[j] = tn;
}

void heap_push(ll d, int node) {
    int i = ++heap_size;
    heap_dist[i] = d;
    heap_node[i] = node;
    while (i > 1) {
        int p = i>>1;
        if (heap_dist[p] <= heap_dist[i]) break;
        heap_swap(p, i);
        i = p;
    }
}

int heap_pop(ll *out_d, int *out_node) {
    if (heap_size == 0) return 0;
    *out_d = heap_dist[1];
    *out_node = heap_node[1];
    heap_swap(1, heap_size--);
    int i = 1;
    while (1) {
        int l = i<<1, r = l|1, smallest = i;
        if (l <= heap_size && heap_dist[l] < heap_dist[smallest]) smallest = l;
        if (r <= heap_size && heap_dist[r] < heap_dist[smallest]) smallest = r;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
    return 1;
}

/* Dijkstra: n = number of nodes (1..n), source = s */
void dijkstra(int n, int s, ll dist[], int prev[]) {
    for (int i = 1; i <= n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    heap_size = 0;
    dist[s] = 0;
    heap_push(0, s);

    while (heap_size) {
        ll d; int u;
        heap_pop(&d, &u);
        if (d != dist[u]) continue; // outdated entry
        for (int ei = head[u]; ei != -1; ei = edges[ei].next) {
            int v = edges[ei].to;
            ll w = edges[ei].weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                heap_push(dist[v], v);
            }
        }
    }
}

/* Print reconstructed path from s to t (recursive) */
void print_path(int prev[], int s, int t) {
    if (t == -1) { printf("unreachable"); return; }
    if (t == s) { printf("%d", s); return; }
    if (prev[t] == -1) { printf("unreachable"); return; }
    print_path(prev, s, prev[t]);
    printf(" -> %d", t);
}

int main() {
    int n, m;
    printf("Enter n (nodes) m (edges):\n");
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) head[i] = -1;
    edge_cnt = 0;

    printf("Enter edges (u v w) each on new line:\n");
    for (int i = 0; i < m; ++i) {
        int u, v; ll w;
        scanf("%d %d %lld", &u, &v, &w);
        add_edge(u, v, w); // for undirected graphs, also add reverse: add_edge(v,u,w);
    }

    int s;
    printf("Enter source node:\n");
    scanf("%d", &s);

    static ll dist[MAXN];
    static int prev[MAXN];

    dijkstra(n, s, dist, prev);

    printf("\nShortest distances from %d:\n", s);
    for (int i = 1; i <= n; ++i) {
        if (dist[i] >= INF/2) printf("  %d: unreachable\n", i);
        else printf("  %d: %lld\n", i, dist[i]);
    }

    printf("\nPaths from %d:\n", s);
    for (int i = 1; i <= n; ++i) {
        printf("  %d -> %d: ", s, i);
        if (dist[i] >= INF/2) printf("unreachable\n");
        else { print_path(prev, s, i); printf(" (cost %lld)\n", dist[i]); }
    }
    return 0;
}


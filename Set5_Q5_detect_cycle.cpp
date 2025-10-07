#include <stdio.h>
#include <stdlib.h>

#define MAX 100
int n, graph[MAX][MAX], visited[MAX], stack[MAX], top, minC = MAX, maxC = 0;

void dfs(int v, int start, int depth) {
    visited[v] = 1;
    stack[top++] = v;

    for (int i = 0; i < n; i++) {
        if (graph[v][i]) {
            if (!visited[i]) dfs(i, start, depth + 1);
            else if (i == start && depth > 1) {
                if (depth < minC) minC = depth;
                if (depth > maxC) maxC = depth;
            }
        }
    }
    visited[v] = 0;
    top--;
}

int main() {
    printf("Enter vertices: ");
    scanf("%d", &n);
    printf("Enter adjacency matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &graph[i][j]);

    for (int i = 0; i < n; i++) dfs(i, i, 0);

    if (minC == MAX) printf("No cycle found.\n");
    else printf("Smallest cycle: %d\nLargest cycle: %d\n", minC, maxC);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;
typedef struct{int u,v; ll w;} E;

int main(){
    int n,m,s,i,j;
    if(scanf("%d %d",&n,&m)!=2) return 0;
    E *edges = malloc(sizeof(E)*m);
    for(i=0;i<m;i++) scanf("%d %d %lld",&edges[i].u,&edges[i].v,&edges[i].w);
    scanf("%d",&s);
    const ll INF = (1LL<<60);
    ll *d = malloc(sizeof(ll)*(n+1));
    for(i=1;i<=n;i++) d[i]=INF;
    d[s]=0;
    for(i=1;i<=n-1;i++){
        int changed=0;
        for(j=0;j<m;j++){
            if(d[edges[j].u]==INF) continue;
            if(d[edges[j].u]+edges[j].w < d[edges[j].v]){
                d[edges[j].v] = d[edges[j].u] + edges[j].w;
                changed=1;
            }
        }
        if(!changed) break;
    }
    int neg=0;
    for(j=0;j<m;j++){
        if(d[edges[j].u]!=INF && d[edges[j].u]+edges[j].w < d[edges[j].v]) { neg=1; break; }
    }
    if(neg){ printf("Negative cycle detected\n"); }
    else{
        for(i=1;i<=n;i++){
            if(d[i]==INF) printf("INF\n"); else printf("%lld\n",d[i]);
        }
    }
    free(edges); free(d);
    return 0;
}


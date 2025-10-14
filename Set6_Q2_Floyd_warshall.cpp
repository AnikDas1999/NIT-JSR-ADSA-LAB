#include <stdio.h>
#include <limits.h>
typedef long long ll;
int main(){
    int n,m,i,j,k; if(scanf("%d %d",&n,&m)!=2) return 0;
    const ll INF = (1LL<<60);
    ll d[n+1][n+1];
    for(i=1;i<=n;i++) for(j=1;j<=n;j++) d[i][j]=(i==j?0:INF);
    while(m--){ int u,v; ll w; scanf("%d %d %lld",&u,&v,&w); d[u][v]=w; }
    for(k=1;k<=n;k++) for(i=1;i<=n;i++) if(d[i][k]<INF) for(j=1;j<=n;j++)
        if(d[k][j]<INF && d[i][k]+d[k][j]<d[i][j]) d[i][j]=d[i][k]+d[k][j];
    for(i=1;i<=n;i++){
        if(d[i][i]<0){ printf("Negative cycle\n"); return 0; }
    }
    for(i=1;i<=n;i++){
        for(j=1;j<=n;j++) printf("%s%lld",(j>1?" ":""), d[i][j]==INF? (long long)0: d[i][j]);
        putchar('\n');
    }
    return 0;
}


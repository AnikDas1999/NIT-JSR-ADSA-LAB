#include <stdio.h>
#include <stdlib.h>
int n, col[30], d1[60], d2[60], p[30];
void pr(){ for(int i=0;i<n;i++){ for(int j=0;j<n;j++) putchar(p[i]==j?'Q':'.'); putchar('\n'); } exit(0); }
void go(int r){
    if(r==n) pr();
    for(int c=0;c<n;c++){
        if(col[c]||d1[r-c+n]||d2[r+c]) continue;
        col[c]=d1[r-c+n]=d2[r+c]=1; p[r]=c;
        go(r+1);
        col[c]=d1[r-c+n]=d2[r+c]=0;
    }
}
int main(){ if(scanf("%d",&n)!=1) return 0; go(0); puts("No solution"); return 0; }


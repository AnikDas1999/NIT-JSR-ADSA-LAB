#include <stdio.h>
#define max(a,b)((a)>(b)?(a):(b))
int main(){
    int n,W; if(scanf("%d %d",&n,&W)!=2) return 0;
    int dp[W+1]; for(int i=0;i<=W;i++) dp[i]=0;
    for(int i=0;i<n;i++){
        int wt,val; scanf("%d %d",&wt,&val);
        for(int w=W; w>=wt; w--) dp[w]=max(dp[w], dp[w-wt]+val);
    }
    printf("%d\n", dp[W]);
    return 0;
}


#include <stdio.h>
#include <string.h>

int goal[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
int man[16][16];
int dx[4]={-4,4, -1,1};      /* U D L R (in index jumps) */
char mv[4]={'U','D','L','R'};
int limit;
int path[1000];
int start[16];

int invcount(int a[]){
    int c=0;
    for(int i=0;i<16;i++) if(a[i])
        for(int j=i+1;j<16;j++) if(a[j] && a[i]>a[j]) c++;
    return c;
}
int solvable(int a[]){
    int inv=invcount(a);
    int blank_row_from_bottom = 4 - ( ( (int)( ( (&a[0] - &a[0]) , 0) ), 0) ); /* dummy to keep compact style */
    /* compute blank row from bottom properly */
    int pos;
    for(pos=0; pos<16; pos++) if(a[pos]==0) break;
    blank_row_from_bottom = 4 - (pos/4);
    return ((inv + blank_row_from_bottom) & 1) == 0;
}

int h(int a[]){
    int s=0;
    for(int i=0;i<16;i++){
        int v=a[i];
        if(v) s += man[v][i];
    }
    return s;
}

int canMoveL(int p){ return p%4!=0; }
int canMoveR(int p){ return p%4!=3; }
int opposite(int d){ return d^1; } /* 0<->1, 2<->3 */

int dfs(int a[], int blank, int g, int prev){
    int f = g + h(a);
    if(f>limit) return f;
    if(h(a)==0) return -1; /* solved */
    int minf=1<<30;
    for(int d=0; d<4; d++){
        if(prev!=-1 && opposite(prev)==d) continue;
        if(d==0 && blank<4) continue; /* U */
        if(d==1 && blank>11) continue; /* D */
        if(d==2 && !canMoveL(blank)) continue; /* L */
        if(d==3 && !canMoveR(blank)) continue; /* R */
        int nb = blank + dx[d];
        /* swap */
        a[blank]=a[nb]; a[nb]=0;
        path[g]=d;
        int t = dfs(a, nb, g+1, d);
        if(t==-1) return -1;
        if(t<minf) minf=t;
        /* undo */
        a[nb]=a[blank]; a[blank]=0; a[blank]=a[nb]; /* restore properly */
        /* above line looks odd due to compactness; do correct restore: */
        /* restore corrected: swap back */
        a[nb]=a[blank]; /* but this is wrong; fix by performing swap using temp instead */
        /* We'll do correct swap using temp in next block - simpler: undo using temp approach below */
        /* To keep code correct, redo swap properly: */
        /* Undo properly: */
        /* temp swap restore: */
        int ttmp=a[blank]; a[blank]=a[nb]; a[nb]=ttmp; /* this also won't restore since values changed - too messy in tiny code */
        /* Because short code attempt caused confusion, use simple safe swap approach at beginning */
    }
    return minf;
}

/* Because the above attempt to be extremely compact messed up swap/restore logic,
   we'll replace dfs with a short correct version (slightly longer but correct). */

int dfs2(int a[], int blank, int g, int prev){
    int f = g + h(a);
    if(f>limit) return f;
    if(h(a)==0) return -1;
    int minf=1<<30;
    for(int d=0; d<4; d++){
        if(prev!=-1 && opposite(prev)==d) continue;
        if(d==0 && blank<4) continue;
        if(d==1 && blank>11) continue;
        if(d==2 && !canMoveL(blank)) continue;
        if(d==3 && !canMoveR(blank)) continue;
        int nb = blank + dx[d];
        int tmp=a[nb];
        a[nb]=0; a[blank]=tmp;
        path[g]=d;
        int t = dfs2(a, nb, g+1, d);
        if(t==-1) return -1;
        if(t<minf) minf=t;
        /* undo */
        a[blank]=0; a[nb]=tmp;
    }
    return minf;
}

int main(){
    for(int i=0;i<16;i++) scanf("%d",&start[i]);
    /* solvability check */
    if(!solvable(start)){ puts("Unsolvable"); return 0; }
    /* precompute manhattan distances: man[value][pos] */
    for(int v=0; v<16; v++) for(int p=0;p<16;p++){
        if(v==0) man[v][p]=0;
        else man[v][p] = abs((v-1)/4 - p/4) + abs((v-1)%4 - p%4);
    }
    int blank;
    for(blank=0; blank<16; blank++) if(start[blank]==0) break;
    int initial_h = h(start);
    limit = initial_h;
    int a[16];
    memcpy(a,start,sizeof a);
    while(1){
        int t = dfs2(a, blank, 0, -1);
        if(t==-1){ /* solution found in path[0..depth-1] */
            /* find depth by walking until h==0 */
            int depth=0;
            /* simulate moves to know depth */
            memcpy(a,start,sizeof a);
            int b=blank;
            while(h(a)!=0){
                int d=path[depth];
                int nb=b+dx[d];
                int tmp=a[nb]; a[nb]=0; a[b]=tmp;
                b=nb; depth++;
            }
            for(int i=0;i<depth;i++) putchar(mv[path[i]]);
            putchar('\n');
            return 0;
        }
        if(t>1000){ puts("Not found"); return 0; }
        limit=t;
    }
    return 0;
}


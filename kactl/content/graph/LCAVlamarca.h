/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: LCA
 * Time: O(X)
 * Status: Tested
 */
#pragma once

//LEMBRAR DE POR O MAKE DEPOIS DE MONTAR A ARVORE
const int N = 5e5+10;
namespace lca_space{
int nlog;
int n;
vector<int> *g;
int pai[N], dist[N]; //pai do no i (raiz = -1)
int st[N][25]; //sparse table - st[i][j] = pai 2^j niveis acima do no i

void dfs(int no, int from, int dac){
    dist[no] = dac;
    for(auto it : g[no]){
        if(it==from) continue;
        pai[it] = no;
        dfs(it,no,dac+1);
    }
}

void make(vector<int> _g[N], int _n, int root){
    g = _g;
    n = _n;
    pai[root] = -1;
    dfs(root,-1,0);
    nlog = 1;
    while((1<<nlog)<n) nlog++;
    assert(nlog<25);
    fr(i,n) fr(j,nlog+1) st[i][j] = -1;
    fr(i,n) st[i][0] = pai[i];
    for(int j = 1; j<=nlog; j++){
        fr(i,n){
            int ant_pai = st[i][j-1];
            if(ant_pai!=-1) st[i][j] = st[ant_pai][j-1];
        }
    }
}

int go_up(int no, int k){
    for(int i = nlog; i>=0; i--){
        if((1<<i)<=k and no!=-1){
            no = st[no][i];
            k-=(1<<i);
        }
    }
    if(k==0) return no;
    return -1;
}

int lca(int p, int q){
    if(dist[p]<dist[q]) swap(p,q);
    p = go_up(p,dist[p]-dist[q]);
    if(p==q) return p;
    for(int i = nlog; i>=0; i--){
        if(st[p][i]!=st[q][i]){
            p = st[p][i];
            q = st[q][i];
        }
    }
    return pai[q];
}

int get_dist(int u, int v){
    return dist[u]+dist[v]-2*dist[lca(u,v)];
}
}; //end lca_space

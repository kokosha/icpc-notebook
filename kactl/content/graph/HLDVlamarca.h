/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: HLD
 * Time: O(X)
 * Status: Tested
 */
#pragma once
template<int N, bool IN_EDGES> struct HLD {
    int t;
    vector<int> *g;
    int pai[N], sz[N], d[N];
    int root[N], pos[N]; /// vi rpos;
    void dfsSz(int no) {
        if (~pai[no]) g[no].erase(find(all(g[no]),pai[no]));
        sz[no] = 1;
        for(auto &it : g[no]) {
            pai[it] = no; d[it] = d[no]+1;
            dfsSz(it); sz[no] += sz[it];
            if (sz[it] > sz[g[no][0]]) swap(it, g[no][0]);
        }
    }
    void dfsHld(int no) {
        pos[no] = t++; /// rpos.pb(no);
        for(auto &it : g[no]) {
            root[it] = (it == g[no][0] ? root[no] : it);
            dfsHld(it); }
    }
    void init(int nor, vector<int> *_g) {
        g = _g;
        root[nor] = d[nor] = t = 0; pai[nor] = -1;
        dfsSz(nor); dfsHld(nor); }
    Seg<N> tree; //lembrar de ter build da seg sem nada
    void changeNode(int v, node val){
        tree.upd(pos[v],val);
    }
    node querySubtree(int v){
        node ans = tree.qry(pos[v]+IN_EDGES,pos[v]+sz[v]);
        return ans;
    }
    template <class Op>
    void processPath(int u, int v, Op op) {
        for (; root[u] != root[v]; v = pai[root[v]]) {
            if (d[root[u]] > d[root[v]]) swap(u, v);
            op(pos[root[v]], pos[v]); }
        if (d[u] > d[v]) swap(u, v);
        op(pos[u]+IN_EDGES, pos[v]);
    }
    node queryPath(int u, int v) { //modificacoes geralmente vem aqui (para hld soma)
        node res; processPath(u,v,[this,&res](int l,int r) {
            res = oper(tree.qry(l,r+1),res); });
        return res;
    }
};

// HLD<N,false> hld;

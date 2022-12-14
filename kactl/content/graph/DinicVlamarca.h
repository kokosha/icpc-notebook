/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: Dinic
 * Time: O(X)
 * Status: Tested
 */
#pragma once
const int MAXV = 3e3+10; // maximo numero de vertices
const int FINF = INT_MAX; // infinite flow

struct Edge {
    int to;
    int cap;
    Edge(int t, int c)
    {
        to = t;
        cap = c;
    }
};

vector<int> adj[MAXV];
vector<Edge> edge;
vector<Edge> eo;
int ptr[MAXV], dinic_dist[MAXV];

// Inserts an edge u->v with capacity c
inline void add_edge(int u, int v, int c)
{
    adj[u].push_back(edge.size());
    edge.push_back(Edge(v, c));
    adj[v].push_back(edge.size());
    edge.push_back(Edge(u, 0)); // modify to Edge(u,c) if graph is non-directed
}

bool dinic_bfs(int _s, int _t)
{
    memset(dinic_dist, -1, sizeof(dinic_dist));
    dinic_dist[_s] = 0;
    queue<int> q;
    q.push(_s);
    while (!q.empty() && dinic_dist[_t] == -1) {
        int v = q.front();
        q.pop();
        for (size_t a = 0; a < adj[v].size(); ++a) {
            int ind = adj[v][a];
            int nxt = edge[ind].to;
            if (dinic_dist[nxt] == -1 && edge[ind].cap) {
                dinic_dist[nxt] = dinic_dist[v] + 1;
                q.push(nxt);
            }
        }
    }
    return dinic_dist[_t] != -1;
}

int dinic_dfs(int v, int _t, int flow)
{
    if (v == _t)
        return flow;
    for (int& a = ptr[v]; a < (int)adj[v].size(); ++a) {
        int ind = adj[v][a];
        int nxt = edge[ind].to;
        if (dinic_dist[nxt] == dinic_dist[v] + 1 && edge[ind].cap) {
            int got = dinic_dfs(nxt, _t, min(flow, edge[ind].cap));
            if (got) {
                edge[ind].cap -= got;
                edge[ind ^ 1].cap += got;
                return got;
            }
        }
    }
    return 0;
}

int dinic(int _s, int _t)
{
    eo = edge; // qnd for fazer o fluxo, guardar como eram as capacidades originais (na vdd isto eh o grafo residual - quanto tem disponivel pra ir de fluxo) para poder recuperar a resposta
    int ret = 0, got;
    while (dinic_bfs(_s, _t)) {
        memset(ptr, 0, sizeof(ptr));
        while ((got = dinic_dfs(_s, _t, FINF)))
            ret += got;
    }
    return ret;
}

// Clears dinic structure
inline void dinic_clear(int n_vertices)
{
    for (int a = 0; a < n_vertices; ++a)
        adj[a].clear();
    edge.clear();
}

typedef tuple<int,int,int> tii;

/* rec_ans recupera resposta do fluxo do dinic
   returna tupla (u,v,c) quanto de fluxo (c) passa de u pra v (direcionado)
   (nao adiciona aresta se nao passa nd de fluxo nela)
   
   Lembrar de por em resposta apenas os vertices necessarios
   (geralmente tenho o source e sink a mais por exemplo)
*/
vector<tii> rec_ans(int n_vertices){
    vector<tii> ans;
    fr(i,n_vertices){
        for(auto &ide : adj[i]){
            if(eo[ide].cap>edge[ide].cap){
                ans.emplace_back(i,edge[ide].to,eo[ide].cap-edge[ide].cap);
            }
        }
    }
    return ans;
}

const int N = 5e5+10;
vector<int> g[N];
vector<int> comp_to_nos[N];
int tempo;
int disc[N]; //primeiro tempo em que nó foi descoberto
int low[N]; //minimo entre disc[no] e low[v] dos vizinhos

//stack e size of stack
int st[N], ss;

//componente do noh i (0 se ainda nao pertence a componente)
//comp[no] : [1,ncomp]
int comp[N], ncomp;

int dfs(int no){
    disc[no] = low[no] = ++tempo;
    st[ss++] = no;
    for(auto it : g[no]){
        if(!disc[it]) low[no] = min(low[no],dfs(it));
        else if(!comp[it]) low[no] = min(low[no],disc[it]);
    }
    if(low[no]==disc[no]){
        comp[no] = ++ncomp;
        while(st[ss-1]!=no) comp[st[--ss]] = comp[no];
        ss--;
    }
    return low[no];
}

int main(){
    //rodar tarjan e definir comps de cada no
    fr(i,n) if(!disc[i]) dfs(i);
    //comp_to_nos, nem sempre necessario, comp 1-indexado
    fr(i,n) comp_to_nos[comp[i]].push_back(i);
}

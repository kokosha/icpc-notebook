/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: SegTreeIterativa
 * Time: O(X)
 * Status: Tested
 */
#pragma once
struct node{
    ll val;
};
node oper(node a, node b){
    return node{a.val+b.val};
}

struct Seg{
node nulo(){
    return node{0};
}
//---------MUDAR ACIMA DISSO GERALMNT--------
    
int n;
vector<node> s;
Seg(){}
void build(){
    for(int i = n-1;i>0;i--){
        s[i] = oper(s[i<<1],s[i<<1|1]);
    }
}
    
Seg(int _n){
    n = _n;
    s = vector<node>(2*n);
    for(int i = n; i<2*n; i++) s[i] = nulo();
    build();
}
    
Seg(vector<ll> v){
    n = sz(v);
    s = vector<node>(2*n);
    for(int i = n; i<2*n; i++) s[i] = node{v[i-n]}; //mudar inicializacao de node a partir de v[i]
    build();
}
//pos 0-indexed (incrementa/faz operacao, nao atualiza/seta)
void upd(int pos, node val){
    pos+=n;
    s[pos] = oper(s[pos],val);
    for(;pos>1;pos>>=1)
        s[pos>>1] = oper(s[pos],s[pos^1]);
    //para atualizar/setar:
    //for(s[pos+=n]=val;pos>1;pos>>=1)
    //    s[pos>>1] = oper(s[pos],s[pos^1]);
}
//array é abstraido para 0-indexed (nas folhas da seg) e [l,r)
node qry(int l, int r){
    node ans = nulo();
    for(l+=n,r+=n;l<r;l>>=1,r>>=1){
        if(l&1) ans = oper(ans,s[l++]);
        if(r&1) ans = oper(ans,s[--r]);
    }
    return ans;
}
}; // end seg

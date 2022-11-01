/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: CHT
 * Time: O(X)
 * Status: Tested
 */
#pragma once
struct Line{
    //reta da forma y=x*m+k
    // p eh onde a reta para, onde deixa de ser a maxima
    mutable ll m,k,p;
    // lembrar de fazer funcao const
    bool operator <(const Line &o) const { return m<o.m;}
    bool operator <(ll x) const { return p<x;}
};
 
struct Cht : multiset<Line,less<>> {
    const ll inf = LLONG_MAX;
    ll div(ll a, ll b){
        return a/b - ( (a^b)<0 and a%b);
    }
    bool bad2(iterator x, iterator y){
        if(y==end()){
            x->p = inf;
            return 0;
        }
        if(x->m==y->m) x->p = (x->k>y->k)? inf : -inf;
        else x->p = div(x->k-y->k, y->m-x->m);
        return x->p>=y->p;
    }
    //tenta adicionar a reta y=m*x+k ao CHTmaximo e o ajusto caso necessario
    void add(ll m, ll k){
        auto z = insert({m,k,0}), y = z++, x = y;
        while(bad2(y,z)) z = erase(z);
        if(x!=begin() and bad2(--x,y)) bad2(x,erase(y));  //no original se usa y=erase(y), mas nao precisa pois y sera redefinido na proxima linha msm
        while((y=x)!=begin() and (--x)->p >= y->p) bad2(x,erase(y));
    }
    //retorna o valor do qual do convex hull de maximo na coordenada x
    ll query(ll x){
        assert(!empty());
        auto l = *lower_bound(x);
        return x*l.m + l.k;
    }
};

int main(){
    Cht cht;
    cht.add(0,0);
    cht.query(y)
}

/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: SufArray
 * Time: O(X)
 * Status: Tested
 */
#pragma once
/*
    Retorna vector p, suffix array
    p[0] eh o indice do menor menor sufixo
    para usar em vector de inteiros, fazer compressao de coordenadas
    para [1,n] e o caracter especial adicionado sera o 0
    Note que n eh incrementado e com caracter especial valores sao [0,n-1]
*/
//vector<int> make_suf(vector<int> s){
vector<int> make_suf(string s){
    s+=(char)0;
    //s.push_back(0);
    int n = sz(s);
    vector<int> p(n), c(n), cnt(max(256,n));
    
    fr(i,n) cnt[s[i]]++;
    for(int i = 1; i<max(n,256); i++) cnt[i]+=cnt[i-1];
    fr(i,n) p[--cnt[s[i]]] = i;
    int nc = 1;
    for(int i = 1; i<n; i++){
        if(s[p[i]]!=s[p[i-1]]) nc++;
        c[p[i]] = nc-1;
    }
    
    vector<int> pn(n), cn(n);
    for(int k = 0; (1<<k)<n; k++){
        fr(i,n) pn[i] = (p[i]-(1<<k)+n)%n;
        fr(i,nc) cnt[i] = 0;
        fr(i,n) cnt[c[i]]++;
        for(int i = 1; i<nc; i++) cnt[i]+=cnt[i-1];
        for(int i = n-1; i>=0; i--) p[--cnt[c[pn[i]]]] = pn[i];
        nc = 1;
        cn[p[0]] = 0;
        for(int i = 1; i<n; i++){
            int v1 = n*c[p[i]] + c[(p[i]+(1<<k))%n];
            int v2 = n*c[p[i-1]] + c[(p[i-1]+(1<<k))%n];
            if(v1!=v2) nc++;
            cn[p[i]] = nc-1;
        }
        c = cn;
    }
    p.erase(p.begin());
    
    return p;
}

//vector<int> make_lcp(vector<int> &s, vector<int> &p){
vector<int> make_lcp(string &s, vector<int> &p){
    int n = sz(s);
    vector<int> rank(n), lcp(n-1);
    
    fr(i,n) rank[p[i]] = i;
    
    int k = 0;
    fr(i,n){
        if(rank[i]==n-1){
            k = 0;
            continue;
        }
        int j = p[rank[i]+1];
        while(i+k<n and j+k<n and s[i+k]==s[j+k]) k++;
        lcp[rank[i]] = k;
        if(k) k--;
    }
    return lcp;
}

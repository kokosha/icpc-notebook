/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: PrefAutomaton
 * Time: O(X)
 * Status: Tested
 */
#pragma once
/*
    Constroi automato de sufixo da string (usa kmp)
        Para string de tamanho n, ha n+1 estados (de [0,n])
        estado 0 é nada da string e n é tudo da string (estou na ultima letra)
        
    prox[c][i] = proximo estado dado que estou no estado i apos adicionar letra c
    
        note que para string "aaaa"
        prox['a'][4] = 4 (continuo na string completa)
*/
int prox[26][N];

vector<int> fpref(string &s){
    vector<int> pref(s.size());
    
    for(int i = 1; i<sz(s); i++){
        int t = pref[i-1];
        while(t and s[i]!=s[t]) t = pref[t-1];
        if(s[i]==s[t]) t++;
        pref[i] = t;
    }
    return pref;
}

void build_aut(string &s){
    vector<int> pref = fpref(s);
    int n = sz(s);
    vector<int> v(n);
    fr(i,n) v[i] = s[i]-'a';
    fr(c,26) prox[c][0] = 0;
    prox[v[0]][0] = 1;
    for(int i = 1; i<=n; i++){
        fr(c,26){
            prox[c][i] = prox[c][pref[i-1]];
        }
        if(i<n) prox[v[i]][i] = i+1;
    }
}

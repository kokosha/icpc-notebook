/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: RecLinear
 * Time: O(X)
 * Status: Tested
 */
#pragma once
/*
    multipllica matriz - VALORES EM MÓDULO
    para matriz n x n complexidade n^3
*/
vector<vector<ll>> mm(vector<vector<ll>> a, vector<vector<ll>> b){
    int l = sz(a);
    int c = sz(b[0]);
    assert(sz(a[0])==sz(b));
    vector<vector<ll>> ans(l,vector<ll>(c));
    fr(i,l){
        fr(j,c){
            ll tot = 0;
            fr(k,a[0].size()){
                tot = (tot+a[i][k]*b[k][j])%MOD;
            }
            ans[i][j] = tot;
        }
    }
    return ans;
}
/*
    Eleva matriz a um expoente que deve ser >=1
    se for zero deveria retornar matriz identidade
*/
vector<vector<ll>> em(vector<vector<ll>> a, ll exp){
    if(exp==1) return a;
    vector<vector<ll>> mid = em(a,exp/2);
    if(exp%2) return mm(mm(mid,mid),a);
    return mm(mid,mid);
}

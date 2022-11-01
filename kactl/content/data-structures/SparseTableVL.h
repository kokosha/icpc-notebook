/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: SparseTable - RMQ
 * Time: O(X)
 * Status: Tested
 */
#pragma once
int log_floor(int n){
    return 31-__builtin_clz(n);
}
ll oper(ll a, ll b){
    return max(a,b);
}
/*
    Sparse table de maximo
        Ou definida de acordo com funcao oper acima
*/
struct sparse_table{
    int exp2;
    int n;
    vector<vector<ll>> mat;
sparse_table(){}
sparse_table(vector<ll> v){
    n = sz(v);
    exp2 = log_floor(n)+1;
    mat.resize(exp2);
    mat[0].resize(n);
    fr(i,n) mat[0][i] = v[i];
    for(int k = 1; k<exp2; k++){
        mat[k].resize(n);
        for(int i = 0; i+(1<<k)<=n; i++){
            mat[k][i] = oper(mat[k-1][i],mat[k-1][i+(1<<(k-1))]);
        }
    }
}
//query fechada [l,r]
ll qry(int l, int r){
    assert(l<=r and l>=0 and r<n);
    int k = log_floor(r-l+1);
    return oper(mat[k][l],mat[k][r-(1<<k)+1]);
}
}; //end sparse_table

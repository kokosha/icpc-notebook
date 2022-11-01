/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: Gauss Elimination - SolveLinear
 * Time: O(X)
 * Status: Tested
 */
#pragma once
/*
    retorno:
        0 - sem solucao
        1 - uma solucao
        2 - infinitas solucoes
        
    resolve sistema - acha X para
        a*X = b
    nos parametros da funcao, b eh a ultima coluna da matriz a
*/
int gauss (vector < vector<mb> > a, vector<mb> & ans) {
    int n = sz(a), m = sz(a[0])-1;
    vector<int> where (m, -1);
    for (int col=0, row=0; col<m && row<n; ++col) {
        int sel = row;
        for (int i=row; i<n; ++i)
            if (a[i][col].val > a[sel][col].val)
                sel = i;
        if (a[sel][col].val==0)
            continue;
        for (int i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;
 
        for (int i=0; i<n; ++i)
            if (i != row) {
                mb c = a[i][col] / a[row][col];
                for (int j=col; j<=m; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }
    ans.assign (m, 0);
    for (int i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    for (int i=0; i<n; ++i) {
        mb sum = 0;
        for (int j=0; j<m; ++j)
            sum += ans[j] * a[i][j];
        if( sum.val != a[i][m].val)
            return 0;
    }
    for (int i=0; i<m; ++i)
        if (where[i] == -1)
            return 2;
    return 1;
}

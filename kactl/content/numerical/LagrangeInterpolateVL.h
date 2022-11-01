/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: Lagrange
 * Time: O(X)
 * Status: Tested
 */
#pragma once
//pode mudar pra double ou mb
vector<frac> interpolate(vector<frac> x, vector<frac> y) {
    int n = sz(x);
    assert(sz(y)==sz(x));
    vector<frac> res(n), temp(n);
    fr(k,n-1) for(int i = k+1; i<n; i++)
        y[i] = (y[i] - y[k]) / (x[i] - x[k]);
    frac last(0,1); temp[0] = frac(1,1);
    fr(k,n) fr(i,n){
        res[i] = res[i] + y[k] * temp[i];
        swap(last, temp[i]);
        temp[i] = temp[i] - last * x[k];
    }
    return res;
}

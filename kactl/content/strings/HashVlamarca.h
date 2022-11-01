/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: Hash
 * Time: O(X)
 * Status: Tested
 */
#pragma once
typedef unsigned long long ull;
ull fpull(ull x, ull e) {
    ull ans = 1;
    for(; e > 0; e /= 2) {
        if(e & 1) ans = ans * x;
        x = x * x;
    }
    return ans;
}
mt19937 rng(time(0));
vector<int> perm;
ull p27[N];
ull inv27[N];
void init_hash(int n){
    fr(i,26) perm.push_back(i+1);
    shuffle(all(perm),rng);
    p27[0] = inv27[0] = 1;
    for(int i = 1; i<n; i++){
        p27[i] = 27*p27[i-1];
        inv27[i] = fpull(p27[i],-1);
    }
}

/*
    Calcula hash de intervalos da string
        primeira letra é digito menos significativo
        string de lowercase english letters
        Base 27 é usada, cada letra é mapeada para [1,26], nao tem 0
        Modulo é (1<<64) - unsigned long long
*/
struct meuhash{
    vector<ull> pref;
    meuhash(){}
    meuhash(string &s){
        assert(sz(s)<N);
        assert(p27[1]*inv27[1]==1);
        pref.resize(sz(s));
        ull cur = 0;
        fr(i,sz(s)){
            cur += p27[i]*perm[s[i]-'a'];
            pref[i] = cur;
        }
    }
    //intervalo fechado [l,r]
    ull gethash(int l, int r){
        assert(l<=r and l>=0 and l<sz(pref) and r>=0 and r<sz(pref));
        l--;
        ull ans = pref[r];
        if(l>=0){
            ans -= pref[l];
            ans *= inv27[l+1];
        }
        return ans;
    }
}; //end hash

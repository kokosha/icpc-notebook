/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: PollardRho
 * Time: O(X)
 * Status: Tested
 */
#pragma once
mt19937 rnd(time(0));
ll grand(ll n){
    return uniform_int_distribution<ll>(0,n-1)(rnd);
}
ll mulmod(ll a, ll b, ll mod){
    if(b<0) return mulmod(a,(b%mod+mod)%mod,mod);
    if(b==0) return 0LL;
    ll ans = (2LL*mulmod(a,b/2,mod))%mod;
    if(b%2==0) return ans;
    return (ans+a)%mod;
}
ll exp_mod(ll a, ll x, ll m) {
    if (x == 0) return 1;
    ll res = exp_mod(a, x/2, m);
    res = mulmod(res, res, m); //(res * res) % m;
    if(x % 2 == 1) res = mulmod(res, a, m); // (res * a) % m
    return res;
}

//Rabin Miller
bool ispp(ll n){
    if(n<=1) return 0;
    if(n<=3) return 1;
    ll s = 0, d = n-1;
    while(d%2==0){
        d/=2;
        s++;
    }
    fr(k,64){
        ll a = grand(n-3)+2;
        ll x = exp_mod(a,d,n);
        if(x!=1 and x!=n-1){
            for(int r = 1;r<s;r++){
                x = mulmod(x,x,n);
                if(x==1) return 0;
                if(x==n-1) break;
            }
            if(x!=n-1) return 0;
        }
    }
    return 1;
}

ll rho(ll n){
    ll d, c = grand(n), x = grand(n),xx=x;
    if(n%2==0){
        return 2;
    }
    do{
        x = (mulmod(x,x,n)+c)%n;
        xx = (mulmod(xx,xx,n)+c)%n;
        xx = (mulmod(xx,xx,n)+c)%n;
        d = gcd(abs(x-xx),n);
    } while(d==1);
    return d;
}

//mapa de primo para frequencia
map<ll,int> F;
void factor(ll n){
    if(n==1) return;
    if(ispp(n)){
        F[n]++;
        return;
    }
    ll d = rho(n);
    factor(d);
    factor(n/d);
    return;
}

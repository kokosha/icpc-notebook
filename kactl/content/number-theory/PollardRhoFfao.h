/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: PollardRhoFfao
 * Time: O(X)
 * Status: Tested
 */
#pragma once
typedef unsigned long long ull;
ull gcd(ull u, ull v) {
    if (u == 0 || v == 0)
        return v ^ u;
    int shift = __builtin_ctzll(u | v);
    u >>= __builtin_ctzll(u);
    do {
        v >>= __builtin_ctzll(v);
        if (u > v) {
            ull t = v;
            v = u;
            u = t;
        }
        v -= u;
    } while (v);
    return u << shift;
}
ull modmul(ull a, ull b, ull M) {
    ll ret = a * b - M * ull(1.L / M * a * b);
    return ret + M * (ret < 0) - M * (ret >= (ll)M);
}
ull modpow(ull b, ull e, ull mod) {
    ull ans = 1;
    for (; e; b = modmul(b, b, mod), e /= 2)
        if (e & 1) ans = modmul(ans, b, mod);
    return ans;
}
bool isPrime(ull n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
        s = __builtin_ctzll(n-1), d = n >> s;
    for (ull a : A) {   // ^ count trailing zeroes
        ull p = modpow(a%n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--)
            p = modmul(p, p, n);
        if (p != n-1 && i != s) return 0;
    }
    return 1;
}
typedef __uint128_t u128;
ull hi(u128 x) { return (x >> 64); }
ull lo(u128 x) { return (x << 64) >> 64; }
struct Mont {
    Mont(ull n) : mod(n) {
        inv = n;
        fr(i,6) inv *= 2 - n * inv;
        r2 = -n % n;
        fr(i,4) if ((r2 <<= 1) >= mod) r2 -= mod;
        fr(i,5) r2 = mul(r2, r2);
    }
    ull reduce(u128 x) const {
        ull y = hi(x) - hi(u128(lo(x) * inv) * mod);
        return ll(y) < 0 ? y + mod : y;
    }
    ull reduce(ull x) const { return reduce(x); }
    ull init(ull n) const { return reduce(u128(n) * r2); }
    ull mul(ull a, ull b) const { return reduce(u128(a) * b); }
    ull mod, inv, r2;
};
ull pollard(ull n) {
    if (n == 9)
        return 3;
    if (n == 25)
        return 5;
    if (n == 49)
        return 7;
    if (n == 323)
        return 17;
    Mont mont(n);
    auto f = [n, &mont](ull x) { return mont.mul(x, x) + 1; };
    ull x = 0, y = 0, t = 0, prd = 2, i = 1, q;
    while (t++ % 32 || gcd(prd, n) == 1) {
        if (x == y)
            x = ++i, y = f(x);
        if ((q = mont.mul(prd, max(x, y) - min(x, y))))
            prd = q;
        x = f(x), y = f(f(y));
    }
    return gcd(prd, n);
}
//Numeros fatorados neste map (primo -> frequencia)
unordered_map<ll, int> mp_fac;
void factor(ull n) {
    if (n == 1)
        return;
    if (isPrime(n))
        mp_fac[n]++;
    else {
        ull x = pollard(n);
        factor(x), factor(n / x);
    }
}
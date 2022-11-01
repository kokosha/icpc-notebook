/**
 * Author: Ludo Pulles, chilli, Simon Lindholm
 * Date: 2019-01-09
 * License: CC0
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf (do read, it's excellent)
   Accuracy bound from http://www.daemonology.net/papers/fft.pdf
 * Description: fft(a) computes $\hat f(k) = \sum_x a[x] \exp(2\pi i \cdot k x / N)$ for all $k$. N must be a power of 2.
   Useful for convolution:
   \texttt{conv(a, b) = c}, where $c[x] = \sum a[i]b[x-i]$.
   For convolution of complex numbers or more than two vectors: FFT, multiply
   pointwise, divide by n, reverse(start+1, end), FFT back.
   Rounding is safe if $(\sum a_i^2 + \sum b_i^2)\log_2{N} < 9\cdot10^{14}$
   (in practice $10^{16}$; higher for random inputs).
   Otherwise, use NTT/FFTMod.
 * Time: O(N \log N) with $N = |A|+|B|$ ($\tilde 1s$ for $N=2^{22}$)
 * Status: somewhat tested
 * Details: An in-depth examination of precision for both FFT and FFTMod can be found
 * here (https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md)
 */
const double PI=acos(-1.0);
namespace fft {
    struct num {
        double x,y;
        num() {x = y = 0;}
        num(double x,double y): x(x), y(y){}
    };
    inline num operator+(num a, num b) {return num(a.x + b.x, a.y + b.y);}
    inline num operator-(num a, num b) {return num(a.x - b.x, a.y - b.y);}
    inline num operator*(num a, num b) {return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);}
    inline num conj(num a) {return num(a.x, -a.y);}
    int base=1;
    vector<num> roots={{0,0}, {1,0}};
    vector<int> rev={0, 1};
    const double PI=acosl(-1.0);
    // always try to increase the base
    void ensure_base(int nbase) {
        if(nbase <= base) return;
        rev.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); i++)
            rev[i] = (rev[i>>1] >> 1) + ((i&1) << (nbase-1));
        roots.resize(1<<nbase);
        while(base<nbase) {
            double angle = 2*PI / (1<<(base+1));
            for(int i = 1<<(base-1); i < (1<<base); i++) {
                roots[i<<1] = roots[i];
                double angle_i = angle * (2*i+1-(1<<base));
                roots[(i<<1)+1] = num(cos(angle_i),sin(angle_i));
            }
            base++;
        }
    }
    void fft(vector<num> &a,int n=-1) {
        if(n==-1) n=a.size();
        assert((n&(n-1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for (int i = 0; i < n; i++) {
            if(i < (rev[i] >> shift)) {
                swap(a[i],a[rev[i] >> shift]);
            }
        }
        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0; i < n; i += 2*k) {
                for(int j = 0; j < k; j++) {
                    num z = a[i+j+k] * roots[j+k];
                    a[i+j+k] = a[i+j] - z;
                    a[i+j] = a[i+j] + z;
                }
            }
        }
    }
    vector<num> fa, fb;
    // multiply with less fft by using complex numbers.
    vector<int> multiply(vector<int> &a, vector<int> &b) {
        int need = a.size() + b.size() - 1;
        int nbase = 0;
        while((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        if(sz > (int)fa.size()) fa.resize(sz);
        for(int i = 0; i < sz; i++) {
            int x = (i < (int)a.size() ? a[i] : 0);
            int y = (i < (int)b.size() ? b[i] : 0);
            fa[i] = num(x, y);
        }
        fft(fa, sz);
        num r(0,-0.25/sz);
        for(int i = 0; i <= (sz>>1); i++) {
            int j = (sz-i) & (sz-1);
            num z = (fa[j] * fa[j] - conj(fa[i] * fa[i])) * r;
            if(i != j) fa[j] = (fa[i] * fa[i] - conj(fa[j] * fa[j])) * r;
            fa[i] = z;
        }
        fft(fa, sz);
        vector<int> res(need);
        for(int i = 0; i < need; i++) res[i] = fa[i].x + 0.5;
        return res;
    }
    vector<int> multiply_mod(vector<int> &a, vector<int> &b, int m, int eq=0) {
        int need = a.size() + b.size() - 1;
        int nbase = 0;
        while((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        if(sz > (int)fa.size()) fa.resize(sz);
        for(int i = 0; i < (int)a.size(); i++) {
            int x = (a[i] % m + m) % m;
            fa[i] = num(x & ((1 << 15) - 1), x >> 15);
        }
        fill(fa.begin() + a.size(), fa.begin() + sz, num{0,0});
        fft(fa, sz);
        if(sz > (int)fb.size()) fb.resize(sz);
        if(eq) copy(fa.begin(), fa.begin() + sz, fb.begin());
        else {
            for(int i = 0; i < (int)b.size(); i++) {
                int x = (b[i] % m + m) % m;
                fb[i] = num(x & ((1 << 15) - 1), x >> 15);
            }
            fill(fb.begin() + b.size(), fb.begin() + sz, num{0,0});
            fft(fb,sz);
        }
        double ratio = 0.25 / sz;
        num r2(0, -1), r3(ratio, 0), r4(0, -ratio), r5(0,1);
        for(int i = 0; i <= (sz>>1); i++) {
            int j = (sz - i) & (sz - 1);
            num a1 = (fa[i] + conj(fa[j]));
            num a2 = (fa[i] - conj(fa[j])) * r2;
            num b1 = (fb[i] + conj(fb[j])) * r3;
            num b2 = (fb[i] - conj(fb[j])) * r4;
            if(i != j) {
                num c1 = (fa[j] + conj(fa[i]));
                num c2 = (fa[j] - conj(fa[i])) * r2;
                num d1 = (fb[j] + conj(fb[i])) * r3;
                num d2 = (fb[j] - conj(fb[i])) * r4;
                fa[i] = c1 * d1 + c2 * d2 * r5;
                fb[i] = c1 * d2 + c2 * d1;
            }
            fa[j] = a1 * b1 + a2 * b2 * r5;
            fb[j] = a1 * b2 + a2 * b1;
        }
        fft(fa, sz); fft(fb, sz);
        vector<int> res(need);
        for(int i = 0; i < need; i++) {
            ll aa = fa[i].x + 0.5;
            ll bb = fb[i].x + 0.5;
            ll cc = fa[i].y + 0.5;
            res[i] = (aa + ((bb%m) << 15) + ((cc%m) << 30))%m;
        }
        return res;
    }
    vector<int> square_mod(vector<int> &a, int m) {
        return multiply_mod(a, a, m, 1);
    }
};
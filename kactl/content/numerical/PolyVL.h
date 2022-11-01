/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: PolyVL
 * Time: O(X)
 * Status: Tested
 */
#pragma once
template<unsigned M_> struct modnum {
    static constexpr unsigned M = M_;
    using ll = long long; using ull = unsigned long long; unsigned x;
    constexpr modnum() : x(0U) {}
    constexpr modnum(unsigned x_) : x(x_ % M) {}
    constexpr modnum(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
    constexpr modnum(ull x_) : x(x_ % M) {}
    constexpr modnum(ll x_) : x(((x_ %= static_cast<ll>(M)) < 0) ? (x_ + static_cast<ll>(M)) : x_) {}
    explicit operator int() const { return x; }
    modnum& operator+=(const modnum& a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }
    modnum& operator-=(const modnum& a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }
    modnum& operator*=(const modnum& a) { x = unsigned((static_cast<ull>(x) * a.x) % M); return *this; }
    modnum& operator/=(const modnum& a) { return (*this *= a.inv()); }
    modnum operator+(const modnum& a) const { return (modnum(*this) += a); }
    modnum operator-(const modnum& a) const { return (modnum(*this) -= a); }
    modnum operator*(const modnum& a) const { return (modnum(*this) *= a); }
    modnum operator/(const modnum& a) const { return (modnum(*this) /= a); }
    modnum operator+() const { return *this; }
    modnum operator-() const { modnum a; a.x = x ? (M - x) : 0U; return a; }
    modnum pow(ll e) const {
        if (e < 0) return inv().pow(-e);
        modnum x2 = x, xe = 1U;
        for (; e; e >>= 1) {
            if (e & 1) xe *= x2;
            x2 *= x2;
        }
        return xe;
    }
    modnum inv() const {
        unsigned a = x, b = M; int y = 1, z = 0;
        while (a) {
            const unsigned q = (b/a), c = (b - q*a);
            b = a, a = c; const int w = z - static_cast<int>(q) * y;
            z = y, y = w;
        } assert(b == 1U); return modnum(z);
    }
    friend modnum inv(const modnum& a) { return a.inv(); }
    template<typename T> friend modnum operator+(T a, const modnum& b) { return (modnum(a) += b); }
    template<typename T> friend modnum operator-(T a, const modnum& b) { return (modnum(a) -= b); }
    template<typename T> friend modnum operator*(T a, const modnum& b) { return (modnum(a) *= b); }
    template<typename T> friend modnum operator/(T a, const modnum& b) { return (modnum(a) /= b); }
    friend bool operator==(const modnum& a, const modnum& b) { return a.x == b.x; }
    friend bool operator!=(const modnum& a, const modnum& b) { return a.x != b.x; }
    friend ostream &operator<<(ostream& os, const modnum& a) { return os << a.x; }
    friend istream &operator>>(istream& in, modnum& n) { ull v_; in >> v_; n = modnum(v_); return in; }
};

template<unsigned M_, unsigned G_, int K_ > struct FFT {
    static_assert(2U <= M_, "Fft: 2 <= M must hold.");
    static_assert(M_ < 1U << 30, "Fft: M < 2^30 must hold.");
    static_assert(1 <= K_, "Fft: 1 <= K must hold.");
    static_assert(K_ < 30, "Fft: K < 30 must hold.");
    static_assert(!((M_ - 1U) & ((1U << K_) - 1U)), "Fft: 2^K | M - 1 must hold.");
    static constexpr unsigned M = M_, M2 = 2U * M_, G = G_;
    static constexpr int K = K_;
    modnum<M> roots[K + 1], inv_roots[K + 1];
    modnum<M> ratios[K], inv_ratios[K];
    constexpr FFT() {
        const modnum<M> g(G);
        for (int k = 0; k <= K; ++k) {
            roots[k] = g.pow((M - 1U) >> k);
            inv_roots[k] = roots[k].inv();
        }
        for (int k = 0; k <= K - 2; ++k) {
            ratios[k] = -g.pow(3U * ((M - 1U) >> (k + 2)));
            inv_ratios[k] = ratios[k].inv();
        } assert(roots[1] == M - 1U);
    }
    void fft(modnum<M>* as, int n) const {
        assert(!(n & (n - 1))); assert(1 <= n); assert(n <= 1 << K);
        int m = n;
        if (m >>= 1) {
            for (int i = 0; i < m; ++i) {
                const unsigned x = as[i + m].x;
                as[i + m].x = as[i].x + M - x;
                as[i].x += x;
            }
        }
        if (m >>= 1) {
            modnum<M> prod = 1U;
            for (int h = 0, i0 = 0; i0 < n; i0 += (m << 1)) {
                for (int i = i0; i < i0 + m; ++i) {
                    const unsigned x = (prod * as[i + m]).x;
                    as[i + m].x = as[i].x + M - x;
                    as[i].x += x;
                }
                prod *= ratios[__builtin_ctz(++h)];
            }
        }
        for (; m;) {
            if (m >>= 1) {
                modnum<M> prod = 1U;
                for (int h = 0, i0 = 0; i0 < n; i0 += (m << 1)) {
                    for (int i = i0; i < i0 + m; ++i) {
                        const unsigned x = (prod * as[i + m]).x;
                        as[i + m].x = as[i].x + M - x;
                        as[i].x += x;
                    }
                    prod *= ratios[__builtin_ctz(++h)];
                }
            }
            if (m >>= 1) {
                modnum<M> prod = 1U;
                for (int h = 0, i0 = 0; i0 < n; i0 += (m << 1)) {
                    for (int i = i0; i < i0 + m; ++i) {
                        const unsigned x = (prod * as[i + m]).x;
                        as[i].x = (as[i].x >= M2) ? (as[i].x - M2) : as[i].x;
                        as[i + m].x = as[i].x + M - x;
                        as[i].x += x;
                    }
                    prod *= ratios[__builtin_ctz(++h)];
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            as[i].x = (as[i].x >= M2) ? (as[i].x - M2) : as[i].x;
            as[i].x = (as[i].x >= M) ? (as[i].x - M) : as[i].x;
        }
    }
    void inverse_fft(modnum<M>* as, int n) const {
        assert(!(n & (n - 1))); assert(1 <= n); assert(n <= 1 << K);
        int m = 1;
        if (m < n >> 1) {
            modnum<M> prod = 1U;
            for (int h = 0, i0 = 0; i0 < n; i0 += (m << 1)) {
                for (int i = i0; i < i0 + m; ++i) {
                    const unsigned long long y = as[i].x + M - as[i + m].x;
                    as[i].x += as[i + m].x;
                    as[i + m].x = (prod.x * y) % M;
                }
                prod *= inv_ratios[__builtin_ctz(++h)];
            }
            m <<= 1;
        }
        for (; m < n >> 1; m <<= 1) {
            modnum<M> prod = 1U;
            for (int h = 0, i0 = 0; i0 < n; i0 += (m << 1)) {
                for (int i = i0; i < i0 + (m >> 1); ++i) {
                    const unsigned long long y = as[i].x + M2 - as[i + m].x;
                    as[i].x += as[i + m].x;
                    as[i].x = (as[i].x >= M2) ? (as[i].x - M2) : as[i].x;
                    as[i + m].x = (prod.x * y) % M;
                }
                for (int i = i0 + (m >> 1); i < i0 + m; ++i) {
                    const unsigned long long y = as[i].x + M - as[i + m].x;
                    as[i].x += as[i + m].x;
                    as[i + m].x = (prod.x * y) % M;
                }
                prod *= inv_ratios[__builtin_ctz(++h)];
            }
        }
        if (m < n) {
            for (int i = 0; i < m; ++i) {
                const unsigned y = as[i].x + M2 - as[i + m].x;
                as[i].x += as[i + m].x;
                as[i + m].x = y;
            }
        }
        const modnum<M> invN = modnum<M>(n).inv();
        for (int i = 0; i < n; ++i) as[i] *= invN;
    }
    void fft(vector<modnum<M>>& as) const { fft(as.data(), int(as.size())); }
    void inverse_fft(vector<modnum<M>>& as) const { inverse_fft(as.data(), int(as.size())); }
    vector<modnum<M>> convolve(vector<modnum<M>> as, vector<modnum<M>> bs) const {
        if (as.empty() || bs.empty()) return {};
        const int len = int(as.size()) + int(bs.size()) - 1;
        int n = 1; for (; n < len; n <<= 1) {}
        as.resize(n); fft(as);
        bs.resize(n); fft(bs);
        for (int i = 0; i < n; ++i) as[i] *= bs[i];
        inverse_fft(as); as.resize(len); return as;
    }
    vector<modnum<M>> square(vector<modnum<M>> as) const {
        if (as.empty()) return {};
        const int len = int(as.size()) + int(as.size()) - 1;
        int n = 1; for (; n < len; n <<= 1) {}
        as.resize(n); fft(as);
        for (int i = 0; i < n; ++i) as[i] *= as[i];
        inverse_fft(as); as.resize(len); return as;
    }
};

using num = modnum<998244353U>;

FFT<998244353U, 3U, 23> fft_data;

// inv: integral, log, exp, pow
constexpr int LIM_INV = 1 << 20;  // @
num invs[LIM_INV], fac[LIM_INV], invFac[LIM_INV];
struct ModIntPreparator {
  ModIntPreparator() {
    invs[1] = 1;
    for (int i = 2; i < LIM_INV; ++i) invs[i] = -((num::M / i) * invs[num::M % i]);
    fac[0] = 1;
    for (int i = 1; i < LIM_INV; ++i) fac[i] = fac[i - 1] * i;
    invFac[0] = 1;
    for (int i = 1; i < LIM_INV; ++i) invFac[i] = invFac[i - 1] * invs[i];
  }
} preparator;

template<unsigned M> struct Poly : public vector<modnum<M>> {
  Poly() {}
  explicit Poly(int n) : vector<modnum<M>>(n) {}
  Poly(const vector<modnum<M>> &vec) : vector<modnum<M>>(vec) {}
  Poly(std::initializer_list<modnum<M>> il) : vector<modnum<M>>(il) {}
  int size() const { return vector<modnum<M>>::size(); }
  num at(long long k) const { return (0 <= k && k < size()) ? (*this)[k] : 0U; }
  int ord() const { for (int i = 0; i < size(); ++i) if ((*this)[i]) return i; return -1; }
  int deg() const { for (int i = size(); --i >= 0;) if ((*this)[i]) return i; return -1; }
  Poly mod(int n) const { return Poly(vector<modnum<M>>(this->data(), this->data() + min(n, size()))); }
  friend std::ostream &operator<<(std::ostream &os, const Poly &fs) {
    os << "[";
    for (int i = 0; i < fs.size(); ++i) { if (i > 0) os << ", "; os << fs[i]; }
    return os << "]";
  }
  Poly &operator+=(const Poly &fs) {
    if (size() < fs.size()) this->resize(fs.size());
    for (int i = 0; i < fs.size(); ++i) (*this)[i] += fs[i];
    return *this;
  }
  Poly &operator-=(const Poly &fs) {
    if (size() < fs.size()) this->resize(fs.size());
    for (int i = 0; i < fs.size(); ++i) (*this)[i] -= fs[i];
    return *this;
  }
  Poly &operator*=(const Poly &fs) {
    if (this->empty() || fs.empty()) return *this = {};
    *this = fft_data.convolve(*this, fs);
    return *this;
  }
  Poly &operator*=(const num &a) {
    for (int i = 0; i < size(); ++i) (*this)[i] *= a;
    return *this;
  }
  Poly &operator/=(const num &a) {
    const num b = a.inv();
    for (int i = 0; i < size(); ++i) (*this)[i] *= b;
    return *this;
  }
  Poly &operator/=(const Poly &fs) {
      auto ps = fs;
      if (size() < ps.size()) return *this = {};
      int s = int(size()) - int(ps.size()) + 1;
      int nn = 1; for (; nn < s; nn <<= 1) {}
      reverse(this->begin(), this->end());
      reverse(ps.begin(), ps.end());
      this->resize(nn); ps.resize(nn);
      ps = ps.inv();
      *this = *this * ps;
      this->resize(s); reverse(this->begin(), this->end());
      return *this;
  }
  Poly &operator%=(const Poly& fs) {
      if (size() >= fs.size()) {
          Poly Q = (*this / fs) * fs;
          this->resize(fs.size() - 1);
          for (int x = 0; x < int(size()); ++x) (*this)[x] -= Q[x];
      }
      while (size() && this->back() == 0) this->pop_back();
      return *this;
  }
  Poly inv() const {
    if (this->empty()) return {};
    Poly b({(*this)[0].inv()}), fs;
    b.reserve(2 * int(this->size()));
    while (b.size() < this->size()) {
        int len = 2 * int(b.size());
        b.resize(2 * len, 0);
        if (int(fs.size()) < 2 * len) fs.resize(2 * len, 0);
        fill(fs.begin(), fs.begin() + 2 * len, 0);
        copy(this->begin(), this->begin() + min(len, int(this->size())), fs.begin());
        fft_data.fft(b);
        fft_data.fft(fs);
        for (int x = 0; x < 2*len; ++x) b[x] = b[x] * (2 - fs[x] * b[x]);
        fft_data.inverse_fft(b);
        b.resize(len);
    }
    b.resize(this->size()); return b;
  }
  Poly differential() const {
      if (this->empty()) return {};
      Poly f(max(size() - 1, 1));
      for (int x = 1; x < size(); ++x) f[x - 1] = x * (*this)[x];
      return f;
  }
  Poly integral() const {
      if (this->empty()) return {};
      Poly f(size() + 1);
      for (int x = 0; x < size(); ++x) f[x + 1] = invs[x + 1] * (*this)[x];
      return f;
  }
  Poly log() const {
      if (this->empty()) return {};
      Poly f = (differential() * inv()).integral();
      f.resize(size()); return f;
  }
  Poly exp() const {
      Poly f = {1};
      if (this->empty()) return f;
      while (f.size() < size()) {
          int len = min(f.size() * 2, size());
          f.resize(len);
          Poly d(len);
          copy(this->begin(), this->begin() + len, d.begin());
          Poly g = d - f.log();
          g[0] += 1;
          f *= g;
          f.resize(len);
      }
      return f;
  }
  Poly pow(int N) const {
      Poly b(size());
      if (N == 0) { b[0] = 1; return b; }
      int p = 0;
      while (p < size() && (*this)[p] == 0) ++p;
      if (1LL * N * p >= size()) return b;
      num mu = ((*this)[p]).pow(N), di = ((*this)[p]).inv();
      Poly c(size() - N*p);
      for (int x = 0; x < int(c.size()); ++x) {
          c[x] = (*this)[x + p] * di;
      }
      c = c.log();
      for (auto& val : c) val *= N;
      c = c.exp();
      for (int x = 0; x < int(c.size()); ++x) {
          b[x + N*p] = c[x] * mu;
      }
      return b;
  }
  Poly operator+() const { return *this; }
  Poly operator-() const {
    Poly fs(size());
    for (int i = 0; i < size(); ++i) fs[i] = -(*this)[i];
    return fs;
  }
  Poly operator+(const Poly &fs) const { return (Poly(*this) += fs); }
  Poly operator-(const Poly &fs) const { return (Poly(*this) -= fs); }
  Poly operator*(const Poly &fs) const { return (Poly(*this) *= fs); }
  Poly operator%(const Poly &fs) const { return (Poly(*this) %= fs); }
  Poly operator/(const Poly &fs) const { return (Poly(*this) /= fs); }
  Poly operator*(const num &a) const { return (Poly(*this) *= a); }
  Poly operator/(const num &a) const { return (Poly(*this) /= a); }
  friend Poly operator*(const num &a, const Poly &fs) { return fs * a; }

  // multipoint evaluation/interpolation
  /* era friend */ static Poly eval(const Poly& fs, const Poly& qs) {
      int N = int(qs.size());
      if (N == 0) return {};
      vector<Poly> up(2 * N);
      for (int x = 0; x < N; ++x) {
          up[x + N] = Poly({0-qs[x], 1});
      }
      for (int x = N-1; x >= 1; --x) {
          up[x] = up[2 * x] * up[2 * x + 1];
      }
      vector<Poly> down(2 * N);
      down[1] = fs % up[1];
      for (int x = 2; x < 2*N; ++x) {
          down[x] = down[x / 2] % up[x];
      }
      Poly y(N);
      for (int x = 0; x < N; ++x) {
          y[x] = (down[x + N].empty() ? 0 : down[x + N][0]);
      }
      return y;
  }
  /* era friend */ static Poly interpolate(const Poly& fs, const Poly& qs) {
      int N = int(fs.size());
      vector<Poly> up(2 * N);
      for (int x = 0; x < N; ++x) {
          up[x + N] = Poly({0-fs[x], 1});
      }
      for (int x = N-1; x >= 1; --x) {
          up[x] = up[2 * x] * up[2 * x + 1];
      }
      Poly E = eval(up[1].differential(), fs);
      vector<Poly> down(2 * N);
      for (int x = 0; x < N; ++x) {
          down[x + N] = Poly({qs[x] * E[x].inv()});
      }
      for (int x = N-1; x >= 1; --x) {
          down[x] = down[2*x] * up[2*x+1] + down[2*x+1] * up[2*x];
      }
      return down[1];
  }
  /* era friend */ static Poly convolve_all(const vector<Poly>& fs, int l, int r) {
      if (r - l == 1) return fs[l];
      else {
          int md = (l + r) / 2;
          return convolve_all(fs, l, md) * convolve_all(fs, md, r);
      }
  }
  static Poly bernoulli(int N) {
      Poly fs(N);
      fs[1] = 1;
      fs = fs.exp();
      copy(fs.begin()+1, fs.end(), fs.begin());
      fs = fs.inv();
      for (int x = 0; x < N; ++x) fs[x] *= fac[x];
      return fs;
  }
  // x(x - 1)(x - 2)...(x - N + 1)
  static Poly stirling_first(int N) {
      if (N == 0) return {1};
      vector<Poly> P(N);
      for (int x = 0; x < N; ++x) P[x] = {-x, 1};
      return convolve_all(P, 0, N);
  }
  static Poly stirling_second(int N) {
      if (N == 0) return {1};
      Poly P(N), Q(N);
      for (int x = 0; x < N; ++x) {
          P[x] = (x & 1 ? -1 : 1) * invFac[x];
          Q[x] = num(x).pow(N) * invFac[x];
      }
      return P * Q;
  }
};

/*
    tested in: https://judge.yosupo.jp/submission/102130
 
    Poly herda de vector de modnum<P>, acessos sao (*this)[i]
        Primo precisa ser fft friendly pra maioria das operacoes (mas posso usar 1 + 7·2^26 e 1 + 5·2^25 e CRT pra recuperar pra outros mods!)
    Ordem do vetor sao os coeficientes do menos pro mais significativo
        a[0]*x^0 + a[1]*x^1 + …
    .deg() do Poly eh o indice do ultimo valor nao nulo (maior expoente)
    .ord() eh o indice do primeiro coef nao nulo
    tds functions a seguir retornam um Poly:
 
    Sao member functions (mas retornam, nao mudam o atual!)
        inv()
        differential()
        integral()
        log()
        exp()
        pow(int N)
        +-*(dividir)
    
    Poderiam ser statics
        eval(Poly a, Poly b)
        interpolate(Poly a, Poly b)
        convolve_all(vector<Poly>, l, r)
        multiplica tds os polys (nlog^2)
        por default usar de 0 a n
        bernoulli(int N)
        stirling_first(int n)
        stirling_second(int n)
 */

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n; cin >> n;
    vector<num> a(n); for(int i=0;i<n;i++) cin >> a[i];
    Poly b(a);
    Poly c = b.exp();
    for(int i=0;i<n;i++) cout << c[i] << " ";
}


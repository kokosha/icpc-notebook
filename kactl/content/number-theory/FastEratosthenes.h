/**
 * Author: Jakob Kogler, chilli, pajenegod
 * Date: 2020-04-12
 * License: CC0
 * Description: Prime sieve.
 * Time: LIM=1e9 $\approx$ 1.5s
 * Status: Stress-tested
 * Details: Despite its n log log n complexity, segmented sieve is still faster
 * than other options, including bitset sieves and linear sieves. This is
 * primarily due to its low memory usage, which reduces cache misses. This
 * implementation skips even numbers.
 *
 * Benchmark can be found here: https://ideone.com/e7TbX4
 *
 * The line `for (int i=idx; i<S+L; idx = (i += p))` is done on purpose for performance reasons.
 * Se https://github.com/kth-competitive-programming/kactl/pull/166#discussion_r408354338
 */
const int MAXN = 10000010;
int lp[MAXN];
vector<int> pr;

void sieve(){
    for (int i = 2; i < MAXN; ++i) {
        if (lp[i] == 0) lp[i] = i, pr.push_back(i);
        for(auto p : pr){
            if(p > lp[i] || i * p >= MAXN) break;
            lp[i * p] = p;
        }
    }
}
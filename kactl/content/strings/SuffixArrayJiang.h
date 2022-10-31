/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: Unknown
 * Source: Suffix array - a powerful tool for dealing with strings
 * (Chinese IOI National team training paper, 2009)
 * Description: Builds suffix array for a string.
 * \texttt{sa[i]} is the starting index of the suffix which
 * is $i$'th in the sorted suffix array.
 * The returned vector is of size $n+1$, and \texttt{sa[0] = n}.
 * The \texttt{lcp} array contains longest common prefixes for
 * neighbouring strings in the suffix array:
 * \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}.
 * The input string must not contain any zero bytes.
 * Time: O(n \log n)
 * Status: stress-tested
 */
struct SuffixArray {
    int n;
    string s;
    vector <int> sa, rk, ht;
    //vector <vector<int> > aux_rk;
    vector <vector<int> > rmq;
    SuffixArray() {}
    SuffixArray(string word) {
        s = word;
        n = word.size();
        //aux_rk.resize(20);
        //for (int i = 0; i < 20; ++i) {
        //    aux_rk[i].resize(n+1);
        //}
        sa.resize(n+1);
        rk.resize(n+1);
        ht.resize(n+1); 
        build(s);
    }
    void build(string s) {
        n = s.size();
        static int set[MAXN + 1], a[MAXN + 1];
        for (int i = 1; i <= n; ++i) set[i] = s[i-1];
        std::sort(set + 1, set + n + 1);
        int *end = std::unique(set + 1, set + n + 1);
        for (int i = 1; i <= n; i++) a[i] = std::lower_bound(set + 1, end, s[i-1]) - set;
        static int fir[MAXN + 1], sec[MAXN + 1], tmp[MAXN + 1], buc[MAXN + 1];
        for (int i = 1; i <= n; i++) buc[a[i]]++;
        for (int i = 1; i <= n; i++) buc[i] += buc[i - 1];
        for (int i = 1; i <= n; i++) rk[i] = buc[a[i] - 1] + 1;
        //int pw = 0;

        for (int t = 1; t <= n; t *= 2) {
            //if need to compare suffix
            //for (int i = 1; i <= n; i++) aux_rk[pw][i] = rk[i];
            //pw++;
            for (int i = 1; i <= n; i++) fir[i] = rk[i];
            for (int i = 1; i <= n; i++) sec[i] = i + t > n ? 0 : rk[i + t];
            std::fill(buc, buc + n + 1, 0);
            for (int i = 1; i <= n; i++) buc[sec[i]]++;
            for (int i = 1; i <= n; i++) buc[i] += buc[i - 1];
            for (int i = 1; i <= n; i++) tmp[n - --buc[sec[i]]] = i;
            std::fill(buc, buc + n + 1, 0);
            for (int i = 1; i <= n; i++) buc[fir[i]]++;
            for (int i = 1; i <= n; i++) buc[i] += buc[i - 1];
            for (int j = 1, i; j <= n; j++) {
                i = tmp[j];
                sa[buc[fir[i]]--] = i;
            }
            bool unique = true;
            for (int j = 1, i, last = 0; j <= n; j++){
                i = sa[j];
                if (!last) rk[i] = 1;
                else if (fir[i] == fir[last] && sec[i] == sec[last]) rk[i] = rk[last], unique = false;
                else rk[i] = rk[last] + 1;
                last = i;
            }
            // This break when all elements is unique
            if (unique) break;
        }
        for (int i = 1, k = 0; i <= n; i++){
            if (rk[i] == 1) k = 0;
            else {
                if (k > 0) k--;
                int j = sa[rk[i] - 1];
                while (i + k <= n && j + k <= n && a[i + k] == a[j + k]) k++;
            }
            ht[rk[i]] = k;
        }
    }
    void build_rmq(int n) {
        rmq.resize(21);

        for (int i = 0; i <= 20; ++i) {
            rmq[i].resize(n+1);
        }
        for (int j = 1; j <= n; j++) {
            rmq[0][j] = ht[j];
        }
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= n; j++) {
                rmq[i][j] = rmq[i-1][j];
                if((j + (1<<(i-1))) <= n)rmq[i][j] = min(rmq[i-1][j], rmq[i-1][j+(1<<(i-1))]);
            }
        }
    }
    int get_lcp(int x, int y) {
        x = rk[x], y = rk[y];
        if(x > y) swap(x, y);
        int aux = y - x - 1;
        int l = 31 - (int)__builtin_clz(aux);
        return min(rmq[l][x+1], rmq[l][y+1 - (1<<l)]);    
    }
    /* compare suffix at i and j with length l
    int compare(int i, int j, int l) {
        int k = 31 - __builtin_clz(l);
        pair<int, int> a = {sa.aux_rk[k][i], sa.aux_rk[k][i+l - (1 << k)]};
        pair<int, int> b = {sa.aux_rk[k][j], sa.aux_rk[k][j+l - (1 << k)]};
        return a == b ? 0 : a < b ? -1 : 1;
    }*/

};
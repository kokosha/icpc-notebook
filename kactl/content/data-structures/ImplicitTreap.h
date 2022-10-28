/**
 * Author: someone on Codeforces
 * Date: 2017-03-14
 * Source: folklore
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data.
 * Time: $O(\log N)$
 * Status: stress-tested
 */

struct Treap { // implicit key (key = index)
    int prior, size;
    int val; //value stored in the array
    int inc, mn;
    bool rev;
    Treap *left, *right;
    Treap() {}
    Treap(int v) {
        prior = rand();
        size = 1;
        val = v;
        inc = 0;
        mn = v;
        rev = false;
        left = right = NULL;
    }
};
inline int size(Treap* t) {
    return (t ? t->size : 0);
}
// Notice that left, right may be NULL
// flag t->inc is set,
// => the subtree of t (t not included) is not up-to-date
// flag t->rev is set,
// => every node in substree of t should
//    swap its 2 children
// Every Treap corresponds to a range in array
inline void push(Treap* t) { 
    if (t->rev) {
        swap(t->left,t->right);
        if (t->left) {
            t->left->rev ^= 1;
        }
        if(t->right) {
            t->right->rev ^= 1;
        }
        t->rev = false;
    }

    if (t->inc) {
        if (t->left) {
            t->left->val += t->inc;
            t->left->inc += t->inc;
            t->left->mn += t->inc;
        }
        if (t->right) {
            t->right->val += t->inc;
            t->right->inc += t->inc;
            t->right->mn += t->inc;
        }
        t->inc = 0;
    }
}
inline void pull(Treap* t) {
    t->size = 1 + size(t->left) + size(t->right);

    t->mn = t->val;
    if (t->left) t->mn = min(t->mn, t->left->mn);
    if (t->right) t->mn = min(t->mn, t->right->mn);
}
int NN = 0;
Treap pool[200000];
inline Treap* new_treap(int val) {
    pool[NN] = Treap(val);
    return &pool[NN++];
}
Treap* merge(Treap* a, Treap* b) {
    if (!a || !b) return (a ? a : b);
    if (a->prior > b->prior) {
        push(a);
        a->right = merge(a->right, b);
        pull(a);
        return a;
    }
    else {
        push(b);
        b->left = merge(a, b->left);
        pull(b);
        return b;
    }
}
// size(a) will be k
// t is unable to use afterwards
void split(Treap* t, Treap*& a, Treap*& b, int k) {
    if (!t) { a = b = NULL; return; }
    push(t);
    if (size(t->left) < k) {
        a = t;
        split(t->right, a->right, b, k - size(t->left) - 1);
        pull(a);
    }
    else {
        b = t;
        split(t->left, a, b->left, k);
        pull(b);
    }
}
void add(Treap*& t, int x, int y, int inc) {
    Treap *a, *b, *c, *d;
    split(t, a, b, y); // t -> a, b
    split(a, c, d, x - 1); // a -> c, d
    d->inc += inc;
    d->val += inc;
    d->mn += inc;
    t = merge(merge(c, d), b);
}
void reverse(Treap*& t, int x, int y) {
    Treap *a, *b, *c, *d;
    split(t, a, b, y); // t -> a, b
    split(a, c, d, x - 1); // a -> c, d
    d->rev ^= 1;
    t = merge(merge(c, d), b);
}
void revolve(Treap*& t, int x, int y, int k) { // go left by k
    int len = y - x + 1;
    Treap *a, *b, *c, *d;
    split(t, a, b, y); // t -> a, b
    split(a, c, d, x - 1); // a -> c, d
    k = k % len;
    Treap *e, *f;
    split(d, e, f, len - k); // d -> e, f
    t = merge(merge(c, merge(f, e)), b);
}
void insert(Treap*& t, int x, int val) {
    Treap *a, *b;
    split(t, a, b, x);
    t = merge(merge(a, new_treap(val)), b);
}
void remove(Treap*& t, int x) {
    Treap *a, *b, *c, *d;
    split(t, a, b, x - 1); // t -> a, b
    split(b, c, d, 1); // b -> c, d
    t = merge(a, d);
}
int get_min(Treap*& t, int x, int y) {
    Treap *a, *b, *c, *d;
    split(t, a, b, y); // t -> a, b
    split(a, c, d, x - 1); // a -> c, d
    int ans = d->mn;
    t = merge(merge(c, d), b);
    return ans;
}
// Treap* root = NULL;
// root = merge(root, new_treap(val)); 
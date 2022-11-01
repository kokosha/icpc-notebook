/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: SegTree GetFirst
 * Time: O(X)
 * Status: Tested
 */
#pragma once
struct node{
    ll soma, mx, mn;
    int l, r;
    ll lazy;
};
node nulo(){
    return node{0,-LLONG_MAX,LLONG_MAX,0,0,0};
}
node oper(node n1, node n2){
    return node{n1.soma+n2.soma,max(n1.mx,n2.mx),min(n1.mn,n2.mn),n1.l,n2.r,0};
}

struct Seg{
int n;
vector<node> s;
vector<ll> v;

// Seta o range. Para Incrementar mudar para +=
void updlazy(int no, ll x){
    if(x==0) return;
    s[no].soma = x*(s[no].r-s[no].l); // +=
    s[no].mx = x; // +=
    s[no].mn = x; // +=
    s[no].lazy = x;
}
//----------------MUDAR ACIMA DISSO (GERALMNT)----------------------

void build(int no, int l, int r){
    if(r-l==1){
        s[no] = node{v[l],v[l],v[l],l,r,0}; //mudar inicializacao a partir de v tmbm
        return;
    }
    int mid = (r+l)/2;
    build(2*no,l,mid);
    build(2*no+1,mid,r);
    s[no] = oper(s[2*no],s[2*no+1]);
}

Seg(vector<ll> _v){
    v = _v;
    n = sz(v);
    s = vector<node>(4*n);
    build(1,0,n);
}

void pass(int no){
    updlazy(2*no,s[no].lazy);
    updlazy(2*no+1,s[no].lazy);
    s[no].lazy = 0;
}

void upd(int lup, int rup, ll x, int no = 1){
    if(rup<=s[no].l or s[no].r<=lup) return;
    if(lup<=s[no].l and s[no].r<=rup){
        updlazy(no,x);
        return;
    }
    pass(no);
    upd(lup,rup,x,2*no);
    upd(lup,rup,x,2*no+1);
    s[no] = oper(s[2*no],s[2*no+1]);
}

node qry(int lq, int rq, int no = 1){
    if(rq<=s[no].l or s[no].r<=lq) return nulo();
    if(lq<=s[no].l and s[no].r<=rq){
        return s[no];
    }
    pass(no);
    return oper(qry(lq,rq,2*no), qry(lq,rq,2*no+1));
}
    
int get_first(int lq, int rq, const function<bool(const node&)> &f, int no = 1){
    if(rq<=s[no].l or s[no].r<=lq) return -1;
    if(!f(s[no])) return -1;
    if(s[no].l+1==s[no].r) return s[no].l;
    pass(no);
    int ans = get_first(lq,rq,f,2*no);
    if(ans!=-1) return ans;
    return get_first(lq,rq,f,2*no+1);
}
    
int get_last(int lq, int rq, const function<bool(const node&)> &f, int no = 1){
    if(rq<=s[no].l or s[no].r<=lq) return -1;
    if(!f(s[no])) return -1;
    if(s[no].l+1==s[no].r) return s[no].l;
    pass(no);
    int ans = get_last(lq,rq,f,2*no+1);
    if(ans!=-1) return ans;
    return get_last(lq,rq,f,2*no);
}
    
}; //end seg

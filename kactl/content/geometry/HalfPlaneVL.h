/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: Half Place Intersection
 * Time: O(X)
 * Status: Tested
 */
#pragma once
/*
    Half plane intersection implementado com precisao inteira
    cuidado overflow
    coordenadas tais que 16*x^4<=1e18 (x<=1e4)
*/
struct pt{
    ll x, y;
    pt(){}
    pt(ll a, ll b){
        x = a, y = b;
    }
    
    pt operator -(pt p2){
        return pt(x-p2.x,y-p2.y);
    }
    
    pt esc(ll e){
        return pt(x*e,y*e);
    }
};

ll operator ^(const pt &p1, const pt &p2){
    return p1.x*p2.y-p1.y*p2.x;
}
struct seg{
    pt p2, p1;
    pt v;
    seg(){}
    seg(pt pl1, pt pl2){
        p2 = pl2, p1 = pl1;
        v = p2-p1;
    }
    seg esc(ll e){
        return seg(p1.esc(e),p2.esc(e));
    }
};
int quad(const pt &v){
    int ans = 0;
    if(v.x<0) ans++;
    if(v.y<0) ans+=2, ans^=1;
    return ans;
}

bool operator <(const seg &a, const seg &b){
    if(quad(a.v)!=quad(b.v)) return quad(a.v)<quad(b.v);
    return (a.v^b.v) > 0;
}

bool operator ==(const seg &a, const seg &b){
    return quad(a.v)==quad(b.v) and (a.v^b.v)==0;
}

/*
    para segmentos paralelos,
    retorna 1 se s1 esta a esquerda de s2
    (ou sao msm reta)
*/
int a_esquerda(seg s1, seg s2){
    pt v2 = s1.p2-s2.p1;
    return (s2.v^v2)>=0;
}

seg oposto(seg s){
    return seg(s.p2,s.p1);
}

auto prox(auto it, list<seg> &l){
    it++;
    if(it==l.end()) it = l.begin();
    return it;
}

auto prev(auto it, list<seg> &l){
    if(it==l.begin()) it = l.end();
    it--;
    return it;
}

struct ptd{
    long double x, y;
    ptd(){}
    ptd(long double a, long double b){
        x = a, y = b;
    }
    ptd operator -(ptd p2){
        return ptd(x-p2.x,y-p2.y);
    }
    long double operator ^(ptd p2){
        return x*p2.y-y*p2.x;
    }
};

int line_intersection(pt p1, pt p2, pt p3, pt p4, pt &p, ll &det){
    ll a1, a2, b1, b2, c1, c2;
    fr(cor,2){
        a1 = p1.y-p2.y, b1 = p2.x-p1.x;
        c1 = p1.x*a1 + p1.y*b1;
        swap(p1,p3);
        swap(p2,p4);
        swap(a1,a2);
        swap(b1,b2);
        swap(c1,c2);
    }
    det = a1*b2-a2*b1;
    assert(det);
    p = pt(c1*b2-b1*c2,a1*c2-a2*c1);
    return 1;
}

int line_intersection(pt p1, pt p2, pt p3, pt p4, ptd &p){
    ll a1, a2, b1, b2, c1, c2;
    fr(cor,2){
        a1 = p1.y-p2.y, b1 = p2.x-p1.x;
        c1 = p1.x*a1 + p1.y*b1;
        swap(p1,p3);
        swap(p2,p4);
        swap(a1,a2);
        swap(b1,b2);
        swap(c1,c2);
    }
    ll det = a1*b2-a2*b1;
    assert(det);
    p = ptd( (1.0l*c1*b2-b1*c2)/det,
             (1.0l*a1*c2-a2*c1)/det );
    return 1;
}

int tira(seg a, seg b, seg c){
    pt pi;
    ll e;
    line_intersection(a.p1,a.p2,c.p1,c.p2,pi,e);
    b = b.esc(e);
    pt v2 = pi-b.p1;
    return (b.v^v2) >= 0;
}

long double area;

void farea(vector<seg> v){
    vector<ptd> vd;
    fr(i,v.size()){
        int i1 = i+1;
        if(i1==v.size()) i1 = 0;
        ptd p;
        line_intersection(v[i].p1,v[i].p2,v[i1].p1,v[i1].p2,p);
        vd.eb(p);
    }
    area = 0;
    frr(i,1,(int)vd.size()-1){
        int i1 = i+1;
        if(i1==vd.size()) i1 = 0;
        ptd v1 = vd[i1]-vd[0];
        ptd v2 = vd[i]-vd[0];
        area += fabs(v1^v2);
    }
    area/=2;
}

/*
    retorno 0 -> area nula
            1 -> area finita
            2 -> area infinita
*/
int hp(vector<seg> v){
    sort(all(v));
    {
        vector<seg> aux;
        int i = 0;
        while(i<v.size()){
            seg cur = v[i];
            while(i+1<v.size() and (v[i+1]==v[i])){
                i++;
                if(a_esquerda(v[i],cur)) cur = v[i];
            }
            aux.eb(cur);
            i++;
        }
        v = aux;
    }
    
    {
        int i = 0, j = 0;
        while(1){
            j = max(j,i+1);
            while(j<v.size() and (v[i].v^v[j].v)>0) j++;
            if(j==v.size()) break;
            if((v[i].v^v[j].v)==0 and a_esquerda(v[i],oposto(v[j]))) return 0;
            i++;
        }
    }
    
    fr(i,v.size()) if((v[i].v^v[(i+1)%v.size()].v) <=0) return 2;
    
    list<seg> l(all(v));
    assert(l.size()>=3);
    
    fr(tt,2) for(auto it = l.begin();it!=l.end();it++){
        fr(cor,2) while(1){
            auto it1 = it;
            auto it2 = prox(it1,l);
            auto it3 = prox(it2,l);
            if(cor){
                it3 = it;
                it2 = prev(it3,l);
                it1 = prev(it2,l);
            }
            if((it1->v^it3->v) <= 0){
                if(!cor and tira(*it2,oposto(*it1),*it3)) return 0;
                else if(cor and tira(*it1,oposto(*it3),*it2)) return 0;
                else break;
            } else{
                if(tira(*it1,*it2,*it3)) l.erase(it2);
                else break;
            }
        }
    }
    v = vector<seg>(all(l));
    farea(v);
    return 1;
}

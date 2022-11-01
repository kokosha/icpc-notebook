/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: ConvexHull
 * Time: O(X)
 * Status: Tested
 */
#pragma once
struct pt //...
bool operator <(const pt &p1, const pt &p2){
    return pll(p1.x,p1.y)<pll(p2.x,p2.y);
}
bool operator ==(const pt &p1, const pt &p2){
    return pll(p1.x,p1.y)==pll(p2.x,p2.y);
}
/*
    gera convex hull em ordem ccw
    pontos colineares são retirados
    no fr(cor,2) porimeiro se faz o lower hull
    depois o upper
    
    unico corner eh se pontos forem tds colineares,
    ai o ch eh degenerado
*/
vector<pt> mch(vector<pt> v){
    sort(all(v));
    v.resize(unique(all(v))-v.begin());
    vector<pt> ans;
    fr(cor,2){
        vector<pt> h;
        fr(i,v.size()){
            while(h.size()>=2){
                pt v1 = h.back()-h[h.size()-2];
                pt v2 = v[i]-h.back();
                if( (v1^v2) > 0 ) break;
                h.pop_back();
            }
            h.eb(v[i]);
        }
        fr(i,(int)h.size()-1){
            ans.eb(h[i]);
        }
        reverse(all(v));
    }
    return ans;
}

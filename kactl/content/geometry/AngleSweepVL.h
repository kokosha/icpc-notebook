/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: AngleSweep
 * Time: O(X)
 * Status: Tested
 */
#pragma once
struct pt //{...}
ll operator ^(const pt &a, const pt &b){
    return a.x*b.y-a.y*b.x;
}
int quad(const pt& p){
    int ans = 0;
    if(p.x<0) ans++;
    if(p.y<0) ans^=1, ans+=2;
    return ans;
}
bool operator ==(const pt &a, const pt &b){
    return quad(a)==quad(b) and (a^b)==0;
}
// Vector by angle comparator - return 0 if they are equal
bool operator <(const pt &a, const pt &b){
    if(quad(a)==quad(b)){
        return (a^b)>0;
    }
    return quad(a)<quad(b);
}

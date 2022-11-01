/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: LineIntersect
 * Time: O(X)
 * Status: Tested
 */
#pragma once
double eps = 1e-7;
int sign(double x){
    if(x>eps) return 1;
    if(x<-eps) return -1;
    return 0;
}

bool eq(double x, double y){
    return sign(x-y)==0;
}

int line_intersection(pt p1, pt p2, pt p3, pt p4, pt &pans){
    double a1, a2, b1, b2, c1, c2;
    fr(cor,2){
        a1 = p1.y-p2.y, b1 = p2.x-p1.x;
        c1 = p1.x*a1 + p1.y*b1;
        swap(p1,p3);
        swap(p2,p4);
        swap(a1,a2);
        swap(b1,b2);
        swap(c1,c2);
    }
    double det = a1*b2-a2*b1;
    if(sign(det)==0){
        if(eq(p1.x*a2+p1.y*b2,c2)) return 0;
        pans = p1;
        return 2;
    }
    pans = pt((1.0l*c1*b2-1.0l*b1*c2)/det,(1.0l*a1*c2-1.0l*a2*c1)/det);
    return 1;
}

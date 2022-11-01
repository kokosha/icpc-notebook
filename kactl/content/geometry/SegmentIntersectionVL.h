/**
 * Author: Victor Lamarca
 * Date: 2015-02-23
 * License: x
 * Source: x
 * Description: SegmentIntersect
 * Time: O(X)
 * Status: Tested
 */
#pragma once
//checa se ponto esta dentro do segmento
bool inptseg(pt a1, pt b1, pt b2){
    pt v1 = (a1-b1), v2 = (b2-b1);
    if(v1^v2) return 0;
    v1 = b1-a1, v2 = b2-a1;
    return (v1*v2)<=0;
}

//checa se segmentos intersectam (bordas inclusas)
bool seg_intersect(pt a1, pt a2, pt b1, pt b2){
    fr(i,2){
        fr(j,2){
            if(inptseg(a1,b1,b2)) return 1;
            swap(a1,a2);
        }
        swap(a1,b1);
        swap(a2,b2);
    }
    fr(cor,2){
        pt v1 = (a1-b1), v2 = (a2-b1), vs = (b2-b1);
        if( 1.0l*(v1^vs)*(v2^vs) >= -0.5 ) return 0;
        swap(a1,b1), swap(a2,b2);
    }
    return 1;
}

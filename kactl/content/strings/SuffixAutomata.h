/**
 * Author: Unknown
 * Date: 2022-10-28
 * License: Unknown
 * Source: Unknown
 * Description: Build suffix automaton
 * Time: O(n \sigma)
 * Status: stress-tested
 */

struct State {
	State *par, *go[26];
	int val;
	int id;
	State(int val = 0) :
		par(NULL), val(0), id(0){
		memset(go, 0, sizeof go);
	}
};
State *root, *last;
State statePool[N * 2], *cur;
void init() {
	cur = statePool;
	root = last = cur++;
}
void extend(int w){
	State *p = last;
	State *np = cur++;
	np->val = p->val + 1;
	while(p && p->go[w] == NULL){
		p->go[w] = np, p = p->par;
	}
	if(p == NULL){
		np->par = root;
	}
	else{
		State *q = p->go[w];
		if(p->val + 1 == q->val){
			np->par = q;
		}
		else{
			State *nq = cur++;
			memcpy(nq->go, q->go, sizeof(q->go));
			nq->val = p->val+1;
			nq->par = q->par;
			q->par = nq;
			np->par = nq;
			while(p && p->go[w] == q){
				p->go[w] = nq, p = p->par;
			}
		}
	}
	last = np;
}
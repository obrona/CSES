#include <bits/stdc++.h>
using namespace std;

// segment tree and coordinate compression

struct Pair {
    int l, r, M;

    Pair(int l, int r, int M): l(l), r(r), M(M) {}
};

class SegTree {
public:
    vector<int> &compressMap;
    int MIN = -1;
    int MAX = 999999999;
    
    vector<Pair> tree;
    int size;
   

    SegTree(vector<int> &compressMap, int n): compressMap(compressMap), tree(4 * n, Pair(MAX, MIN, 0)), size(n) {
        build(0, 0, size - 1);
    }

    void build(int p, int s, int e) {
        tree[p].M = getRealPos(e) - getRealPos(s);
        if (s == e) return;

        int m = (s + e) >> 1;
        build((p << 1) + 1, s, m);
        build((p << 1) + 2, m + 1, e);
    }

    int getRealPos(int x) {
        return compressMap[x];
    }
    
    void insert(int p, int s, int e, int idx) {
        if (s == e) {
            tree[p].l = idx;
            tree[p].r = idx;
            tree[p].M = 0;
        } else {
            int m = (s + e) >> 1;
            if (idx <= m) insert((p << 1) + 1, s, m, idx);
            else insert((p << 1) + 2, m + 1, e, idx);

            tree[p].l = min(tree[(p << 1) + 1].l, tree[(p << 1) + 2].l);
            tree[p].r = max(tree[(p << 1) + 1].r, tree[(p << 1) + 2].r);
            
            // option 1 means left node and right node has no traffic lights at all
            // if leftmost traffic light exists, then rightmost traffic light must also exist, as it can be
            // the leftmost traffic light
            int leftTraffic = (tree[(p << 1) + 1].r == MIN) ? getRealPos(s) : getRealPos(tree[(p << 1) + 1].r);
            int rightTraffic = (tree[(p << 1) + 2].l == MAX) ? getRealPos(e) : getRealPos(tree[(p << 1) + 2].l);
            
            tree[p].M = max(tree[(p << 1) + 1].M ,max(tree[(p << 1) + 2].M, rightTraffic - leftTraffic));
        }
    }


    int getMax() {
        
        return tree[0].M;
    }

};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int x, n;
    cin >> x >> n;

    unordered_map<int, int> compressMap;
    vector<int> store;
    vector<int> trafficLights({0, x}); // need to include the start and end points
    for (int i = 0; i < n; i ++) {
        int pos;
        cin >> pos;
        trafficLights.push_back(pos);
        store.push_back(pos);
    }
    
    sort(trafficLights.begin(), trafficLights.end());
    
    for (int i = 0; i < trafficLights.size(); i ++) {
        compressMap[trafficLights[i]] = i;
    }

    SegTree st(trafficLights, trafficLights.size());
    

    for (int i = 0; i < store.size(); i ++) {
        
        st.insert(0, 0, st.size - 1, compressMap[store[i]]); // all elem distinct so compresssed idx is simply the pos
        cout << st.getMax() << " ";
    }
}

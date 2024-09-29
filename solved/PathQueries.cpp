#include <bits/stdc++.h>
using namespace std;
#define LEN 1600005
using ll = long long;

// bascially we do the euler tour, where for each vertex we calculate the start, end time using dfs, and put them into an array
// using the start/end time as the position, except whem we put in a vertex for its end time, put in -weight, not weight
// to query path uv, start(u) < start(v), find lca, then query [start(lca), start(u)] + [start(lca), start(v)]
// int he flattened query, any vertices not in the path will not contribute to the sum, as its weight is cancelled out by its negative
// hence we can use segment tree for querying
// for updating, we update start(u) with newVal and end(u) with -newVal

class LCA {
public:
    int MAX = 18; 
    vector<vector<int>> binLift;
    vector<int> depth;

    LCA() {};

    LCA(int n, vector<vector<int>>& adj): binLift(n, vector<int>(MAX + 1, -1)), depth(n, 0) {
        processBinLift(adj, 1, 1, 0);
    }

    void processBinLift(vector<vector<int>>& adj, int v, int p, int d) {
        depth[v] = d;
        if (v != p) {
            binLift[v][0] = p;
            for (int i = 1; i <= MAX; i ++) {
                int curr = binLift[binLift[v][i - 1]][i - 1]; // no need to check for binLift[v][i - 1], as we are in ith iteration if 2**(i-1) parent of v is valid
                if (curr == -1) break;
                binLift[v][i] = curr;
            }
        }
        
        for (int c : adj[v]) {
            if (c != p) processBinLift(adj, c, v, d + 1);
        }
    }

    int getIthParent(int v, int jumps) {
        int i = 0;
        while (jumps > 0) {
            if (jumps & 1 && binLift[v][i] != -1) v = binLift[v][i];
            jumps = jumps >> 1;
            i ++;
        }
        return v;
    }

    int getLca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
        u = getIthParent(u, depth[u] - depth[v]);
        if (u == v) return u;
        
        for (int i = MAX; i >= 0; i --) {
            if (binLift[u][i] != binLift[v][i]) {
                u = binLift[u][i];
                v = binLift[v][i];
            } 
        }
        return binLift[u][0];
    }
};

class SegTree {
public:
    ll tree[LEN];
    int n;

    SegTree() {};
    
    SegTree(vector<int>& arr): n(arr.size()) {
        build(0, 0, n - 1, arr);
    }

    void build(int p, int s, int e, vector<int>& arr) {
        if (s == e) {
            tree[p] = (ll) arr[s];
            return;
        } 
        int m = (s + e) / 2;
        build(2 * p + 1, s, m, arr);
        build(2 * p + 2, m + 1, e, arr);
        tree[p] = tree[2 * p + 1] + tree[2 * p + 2];
    }

    void update(int p, int s, int e, int idx, int val) {
        if (s == e) tree[p] = (ll) val;
        else {
            int m = (s + e) / 2;
            if (idx <= m) update(2 * p + 1, s, m, idx, val);
            else update(2 * p + 2, m + 1, e, idx, val);
            tree[p] = tree[2 * p + 1] + tree[2 * p + 2];
        }
    }

    ll query(int p, int s, int e, int l, int r) {
        if (s == e || (s == l && e == r)) return tree[p];
        else {
            int m = (s + e) / 2;
            if (r <= m) return query(2 * p + 1, s, m, l, r);
            else if (l > m) return query(2 * p + 2, m + 1, e, l, r);
            else return query(2 * p + 1, s, m, l, m) + query(2 * p + 2, m + 1, e, m + 1, r);
        }
    }

};

LCA lca;
SegTree segtree;
vector<int> weights;
vector<vector<int>> startEnd;
vector<vector<int>> adj;
vector<int> flat;

// returns the next available time
int dfs(int v, int p, int t) {
    startEnd[v][0] = t;
    flat.push_back(weights[v]);
    t ++;
    
    for (int c: adj[v]) {
        if (c != p) t = dfs(c, v, t);
    }
    
    startEnd[v][1] = t;
    flat.push_back(-weights[v]);
    t ++;
    return t;
}

void update(int v, int val) {
    weights[v] = val;
    int start = startEnd[v][0];
    int end = startEnd[v][1];
    segtree.update(0, 0, segtree.n - 1, start, val);
    segtree.update(0, 0, segtree.n - 1, end, -val);
}

ll query(int u, int v) {
    int lcca = lca.getLca(u, v);
    return segtree.query(0, 0, segtree.n - 1, startEnd[lcca][0], startEnd[u][0]) 
        + segtree.query(0, 0, segtree.n - 1, startEnd[lcca][0], startEnd[v][0]) - (ll) weights[lcca];
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, q;
    cin >> n >> q;
    adj = vector(n + 1, vector<int>());
    startEnd = vector(n + 1, vector<int>(2, 0));
    weights = vector(n + 1, 0);

    for (int i = 1; i <= n; i ++) {
        int w;
        cin >> w;
        weights[i] = w;
    }

    for (int i = 0; i < n - 1; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    lca = LCA(n + 1, adj);
    dfs(1, 1, 0);
    segtree = SegTree(flat);

    for (int i = 0; i < q; i ++) {
        int t, s, x;
        cin >> t;
        if (t == 1) {
            cin >> s >> x;
            update(s, x);
        } else {
            cin >> s;
            cout << query(1, s) << "\n";
        }
    }
}
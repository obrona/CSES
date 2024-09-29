#include <bits/stdc++.h>
using namespace std;
#define LEN 200005;

// notice that if u, v are already connected, no need to add the road, even if on day i we build road uv
// so graph is a tree, and query between u, v what is the maximum weight of an edge in path uv, where weight is the day 
// the road is built
// since no modification to edges, we can modify LCA to store the 2 ** i parent and also the max edge from u to the 2 ** i parent

struct Pair {
    int u;
    int w;

    Pair(int u, int w): u(u), w(w) {};
};

struct LCA {
    int MAX;
    vector<vector<Pair>> binLift;
    vector<int> depth;

    LCA() {};

    LCA(int n, vector<vector<Pair>>& adj): MAX((int) ceil(log(n) / log(2))), binLift(n, vector(MAX + 1, Pair(-1, -1))), depth(n) {
        //dfs(1, 1, 0, 0, adj);
        //dfs(0, 0, 0, 0, adj);
    };

    void dfs(int v, int p, int w, int d, vector<vector<Pair>>& adj) {
        depth[v] = d;
        if (v != p) {
            binLift[v][0].u = p;
            binLift[v][0].w = w;
            for (int i = 1; i <= MAX; i ++) {
                // invariant, in ith iteration, the 2**(i - 1) parent of u exists
                int parent = binLift[v][i - 1].u;
                int nextParent = binLift[parent][i - 1].u;

                if (nextParent == -1) break;
                int maxW = max(binLift[v][i - 1].w, binLift[parent][i - 1].w);
                binLift[v][i].u = nextParent;
                binLift[v][i].w = maxW;
            }
        }
        for (Pair& c : adj[v]) {
            if (c.u == p) continue;
            dfs(c.u, v, c.w, d + 1, adj);
        }
    }

    int getMaxWeight(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        
 
        int res = 0;
        int diff = depth[u] - depth[v];
        int pow = 0;
        while (diff > 0) {
            if (diff & 1) {
                res = max(res, binLift[u][pow].w);
                u = binLift[u][pow].u;
            }
            pow ++;
            diff >>= 1;

        }

        for (int i = MAX; i >= 0; i --) {
            Pair& pu = binLift[u][i];
            Pair& pv = binLift[v][i];
            if (pu.u != pv.u) {
                res = max(res, max(pu.w, pv.w));
                u = pu.u;
                v = pv.u;
            }
        }

        
        if (u != v) {
            if (binLift[u][0].u != -1) res = max(res, max(binLift[u][0].w, binLift[v][0].w));
            else res = 999999999; // u, v are in 2 different component
        }
        return res;
    }
};

struct UnionFind {
    vector<int> parent;
    vector<int> weight;

    UnionFind() {};

    UnionFind(int n): parent(n, 0), weight(n, 1) {
        for (int i = 0; i < n; i ++) {
            parent[i] = i;
        }
    }

    int find(int u) {
        if (parent[u] == u) return u;
        int p = find(parent[u]);
        parent[u] = p;
        return p;
    }

    void join(int u, int v) {
        int pu = find(u);
        int pv = find(v);
        if (pu == pv) return;
        if (weight[pu] < weight[pv]) {
            parent[pu] = pv;
            weight[pv] += weight[pu];
        } else {
            parent[pv] = pu;
            weight[pu] += weight[pv];
        }
    }
};

UnionFind uf;
LCA lca;
vector<vector<Pair>> adj;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, m, q;
    cin >> n >> m >> q;
    uf = UnionFind(n + 1);
    adj = vector(n + 1, vector<Pair>());

    for (int i = 1; i <= m; i ++) {
        int a, b;
        cin >> a >> b;

        // if a, b are not connected yet, add the edge, if not then edge is redundant
        if (uf.find(a) != uf.find(b)) {
            adj[a].push_back(Pair(b, i));
            adj[b].push_back(Pair(a, i));
            uf.join(a, b);
        }
    }

    lca = LCA(n + 1, adj);
    for (int i = 1; i <= n; i ++) {
        // do binary lifting for each component (which is a tree)
        if (uf.find(i) == i) {
            lca.dfs(i, i, 0, 0, adj);
        }
    }

    for (int i = 0; i < q; i ++) {
        int a, b;
        cin >> a >> b;
        int res = lca.getMaxWeight(a, b);
        if (res < 999999999) cout << res << "\n";
        else cout << -1 << "\n";
    }

   
    
}


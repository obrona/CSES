
#include <bits/stdc++.h>
#define LEN 200005
using namespace std;

// need to use heavy light decompostion to answer query in (logn)**2 and update in logn
// heavy light decomposition, heavy edge is from v to the child with the most weight, rest of edges from v to other child is light
// clearly, everytime pass light edge, num of nodes is halved. So max logn changes to heavy "chains"
// each pathj query u to v is split into path (u, lca) and (lca, u)
// so logn chain, each query on chain is logn, so T.C for each query is (logn)**2

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

// got this code on codeforces
struct SegTree {
    int n;  // array size
    int* t;

    SegTree(const vector<int>& arr) : n(arr.size()) {
        t = new int[2 * n];
        for (int i = 0; i < arr.size(); i ++) {
            modify(i, arr[i]);
        }
    }

    void build() {  // build the tree
        for (int i = n - 1; i > 0; --i) t[i] = t[i<<1] + t[i<<1|1];
    }

    void modify(int p, int value) {  // set value at position p
        for (t[p += n] = value; p > 1; p >>= 1) t[p>>1] = max(t[p], t[p^1]);
    }

    int query(int l, int r) {  // max on interval [l, r)
        int res = 0;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) res = max(res, t[l++]);
            if (r&1) res = max(res, t[--r]);
        }
        return res;
    }
};


LCA lca;
int nodeWeights[LEN];
//vector<int> weights; // no need to store weights
int parents[LEN];
int heavyChild[LEN];
vector<vector<int>> adj;
int segTreeStart[LEN];
vector<vector<int>> segTreeMap; // for each node, I need to know which segtree and also it indice in the 'chain'
vector<int> storeWeights;

vector<SegTree*> segtrees;


int dfsWeights(int v, int p) {
    parents[v] = p;
    int sum = 1;

    int heavyC = -1;
    int heavy = 0;
    
    for (int c : adj[v]) {
        if (c != p) {
            int w = dfsWeights(c, v);
            sum += w;
            if (w > heavy) {heavy = w; heavyC = c;}
        }
    }
    
    //weights[v] = sum;
    heavyChild[v] = heavyC;
    return sum;
}

void hld(int v, int p, int idx) {
    if (storeWeights.size() == 0) segTreeStart[segtrees.size()] = v;
    
    storeWeights.push_back(nodeWeights[v]);
    segTreeMap[v][0] = segtrees.size();
    segTreeMap[v][1] = idx;
    
    // check if leaf
    if (adj[v].size() == 1 && v != p) {
        segtrees.push_back(new SegTree(storeWeights));
        storeWeights.clear();
    } else {
        hld(heavyChild[v], v, idx + 1);
        for (int c : adj[v]) {
            if (c != p && c != heavyChild[v]) hld(c, v, 0);
        }
    }
}

void update(int v, int val) {
    const vector<int>& id = segTreeMap[v];
    
    int size = segtrees[id[0]]->n;
    segtrees[id[0]]->modify(id[1], val);
}

int getRes(int start, int end) {
    const vector<int>&  endId = segTreeMap[end];
    int res = -1;
    vector<int> startId = segTreeMap[start];
    while (true) {
        // end and curr start is in same chain already
        if (startId[0] == endId[0]) {
            res = max(res, segtrees[startId[0]]->query(endId[1], startId[1] + 1));
            break;
        } else {
            res = max(res, segtrees[startId[0]]->query(0, startId[1] + 1));
            int nodeStart = segTreeStart[startId[0]]; // get start node of chain
            int p = parents[nodeStart]; // get parent of chain
            startId = segTreeMap[p]; // change to startId of last node of next chain to query
        }
    }
    return res;
}

int query(int u, int v) {
    int lcca = lca.getLca(u, v);
    //cout << u << " " << v << " " << lcca << endl;
    return max(getRes(u, lcca), getRes(v, lcca));
}



int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, q;
    cin >> n >> q;
    
    //nodeWeights = vector(n + 1, 0);
    //weights = vector(n + 1, 0);
    //parents = vector(n + 1, 0);
    //heavyChild = vector(n + 1, 0);
    adj = vector(n + 1, vector<int>());
    //segTreeStart = vector(n + 1, 0);
    segTreeMap = vector(n + 1, vector<int>(2, -1));

    for (int i = 1; i <= n; i ++) {
        int w;
        cin >> w;
        nodeWeights[i] = w;
    }

    for (int i = 1; i <= n - 1; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    //for_each(adj[1].begin(), adj[1].end(), [](int x) -> void {cout << x << " ";});

    dfsWeights(1, 1);
    hld(1, 1, 0);
    lca = LCA(n + 1, adj);
    
    for (int i = 0; i < q; i ++) {
        int t, s, x;
        cin >> t >> s >> x;
        if (t == 1) update(s, x);
        else {
            //query(s, x);
            cout << query(s, x) << " ";
        }
    }
}



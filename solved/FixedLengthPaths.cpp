#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define LEN 200005

// use centroid decomposition
// helper returns, for each vertex, a set detailing how many paths of length i
// find centroid, recurse

int N;
int k;
int depth[LEN];
vector<vector<int>> adj;
bool removed[LEN];
int weights[LEN];

ll ans = 0; // after each call to helper ends, ans contains how many paths of length k in that subtree, rooted at v

int dfs1(int v, int p, int d, vector<int>& store) {
    if (store.size() < d + 1) store.resize(d + 1, 0);
    store[d] ++;

    int w = 1;
    for (int c : adj[v]) {
        if (c == p || removed[c]) continue;
        w += dfs1(c, v, d + 1, store);
    }
    
    weights[v] = w;
    return w;
}

int findCentroid(int v, int p, int lim) {
    for (int c : adj[v]) {
        if (c == p || removed[c]) continue;
        if (weights[c] > (lim >> 1)) return findCentroid(c, v, lim);
    }
    return v;
}

ll mergeRes(vector<int>& child, vector<int>& pathStore) {
    ll res = 0;
    int lim = min(k, (int) child.size());
    
    for (int i = 0; i < lim; i ++) {
        int opp = k - i - 1;
        if (pathStore.size() <= opp) continue;
        res += (ll) child[i] * pathStore[opp];
    }

    for (int i = 0; i < lim; i ++) {
        if (pathStore.size() <= i + 2) pathStore.resize(i + 2);
        pathStore[i + 1] += child[i];
    }
    
    return res;
}

vector<int> helper(int v, int p) {
    vector<int> store;
    int w = dfs1(v, p, 0, store);

    ll tempAns = 0;
    
    vector<int> pathStore(1);
    pathStore[0] = 1;
    
    int cen = findCentroid(v, p, w);
    removed[cen] = true;
    
    for (int c : adj[cen]) {
        // if c == p, it is till valid
        if (removed[c]) continue;
    
        vector<int>&& res = helper(c, cen);

        tempAns += ans;
        tempAns += mergeRes(res, pathStore);
    }
    
   
    ans = tempAns;
    return store;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> N >> k;

    //depth = vector(N, 0);
    adj = vector(N, vector<int>());
    //removed = vector(N, false);
    //weights = vector(N, 0);

    for (int i = 0; i < N - 1; i ++) {
        int a, b;
        cin >> a >> b;
        a --; b --;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    helper(0, 0);
    
    cout << ans << endl;
}
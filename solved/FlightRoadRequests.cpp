#include <bits/stdc++.h>
using namespace std;

// we consider a connected componenent DG (i.e consider the edges to be undirected)
// if the DG has a cycle -> #edges is n
// because we can just do this 1->2->3 ... ->n->1, whatever SCC it has, the entire component is now a SCC
// if it is a DAG, then we can have a topological order, so #edges is n - 1
// because, let say topo order is t1 t2 t3 ... tn
// we can do t1->t2->t3 ... ->tn, and since this is a topo order, will preserve the "connectivity" of the original DAG
// so if a can go to b, in the new set of edges, a and still go back to b

vector<bool> visited;
vector<bool> processed;
vector<vector<int>> adj;

vector<bool> visited2;
vector<vector<int>> undirAdj;
vector<int> store;

bool dfs(int v) {
    if (visited[v] && !processed[v]) return true;
    if (visited[v] && processed[v]) return false;

    visited[v] = true;
    bool hasCycle = false;
    for (int c : adj[v]) {
        bool p = dfs(c);
        hasCycle |= p;
    }
    processed[v] = true;
    return hasCycle;
}

int dfs2(int v) {
    if (visited2[v]) return 0;

    visited2[v] = true;
    store.push_back(v);
    int sum = 1;
    for (int c : undirAdj[v]) {
        sum += dfs2(c);
    }
    return sum;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, m;
    cin >> n >> m;
    visited = vector(n + 1, false);
    processed = vector(n + 1, false);
    adj = vector(n + 1, vector<int>());
    visited2 = vector(n + 1, false);
    undirAdj = vector(n + 1, vector<int>());

    for (int i = 0; i < m; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);

        undirAdj[a].push_back(b);
        undirAdj[b].push_back(a);
    }

    int res = 0;
    for (int v = 1; v <= n; v ++) {
        if (visited[v]) continue;

        int sum = dfs2(v);
        bool hasCycle = accumulate(store.begin(), store.end(), false, [](bool prev, int node) {return prev || dfs(node);});

        res += max(0, sum - !hasCycle);
        store.clear();
        
        //cout << v << " " << sum << " " << hasCycle << endl;
    }
    cout << res << endl;
    

}
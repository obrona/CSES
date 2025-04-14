#include <bits/stdc++.h>
using namespace std;
constexpr int LEN = 200005;

// use tarjan algorithm
// if > 1 SCC, ans is NO, then print 2 nodes, each from different SCC, in reverse topological order

array<bool, LEN> visited = {false};
array<bool, LEN> inStack = {false};
array<int, LEN> ids = {-1};
array<int, LEN> low = {-1};
vector<int> stac;
int sccid = 0;

vector<vector<int>> adj;

void dfs(int v, int id) {
    visited[v] = true;
    inStack[v] = true;
    stac.push_back(v);

    ids[v] = low[v] = id;
    
    for (int c : adj[v]) {
        if (!visited[c]) {
            dfs(c, id + 1); // this is wrong !!, id must be a global count
        }

        if (inStack[c]) {
            low[v] = min(low[v], low[c]);
        }
    }

    // root of SCC, not unique, depends on dfs
    if (low[v] == id) {
        
        while (1) {
            int u = stac.back(); stac.pop_back();
            inStack[u] = false;
            ids[u] = sccid;
            if (u == v) break;
        }
        sccid++;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;

    adj = vector(N + 1, vector<int>());
    for (int i = 0; i < M; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
    }
    
    for (int i = 1; i <= N; i ++) {
        if (visited[i]) continue;
        dfs(i, 1);
    }

    if (sccid == 1)  {
        cout << "YES" << endl;
        return 0;
    } else {
        int a, b;
        for (int i = 1; i <= N; i ++) {
            if (ids[i] == 0) {
                a = i;
            } else if (ids[i] == sccid - 1) {
                b = i;
            }
        }
        
        cout << "NO" << endl;
        cout << a << " " << b << endl;
    }

}

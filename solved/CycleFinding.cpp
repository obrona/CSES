#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// use bellman ford
// not that the negative cycle can be 'found' in the N - i iterations, i >= 1
// so not neccessarily that the dest node in the last iteration whose dist can be lowered is the start node of the cyle

// why bellman ford find negative cycle with just extra 1 iterations
// because a path with a negative cycle contains at most N edges

// invaariant of bellman ford. At the end of the i iterations (1 indexed), dist[node] constains shortest path 
// from src to node with i edges

// use routing table to reconstruct the cycle

ll INF = (1LL << 40);

int n, m;
bool hasNegCycle = false;
int startNode;
ll arr[2501];
int route[2501];
vector<vector<int>> edges;

void process() {
    arr[0] = 0;
    for (int i = 1; i < n + 1; i ++) arr[i] = INF;
}

void bellmanFord() {
    for (int i = 1; i <= n; i ++) {
        for (vector<int>& e : edges) {
            int src = e[0];
            int dest = e[1];
            ll w = e[2];
            if (arr[src] + w < arr[dest]) {
                arr[dest] = w + arr[src];
                route[dest] = src;
            }
        }
    }

    // last iteration
    for (vector<int> e : edges) {
        int src = e[0];
        int dest = e[1];
        ll w = e[2];
        if (arr[src] + w < arr[dest]) {
            hasNegCycle = true;
            startNode = dest;
            route[dest] = src;
            break;
        }
    }
   
}

vector<int> getNegCycle() {
    vector<bool> visited(n + 1, false);
    int curr = startNode;
    int s;
    while (true) {
        visited[curr] = true;
        curr = route[curr];
        if (visited[curr]) {
            s = curr;
            break;
        }
    }

    vector<int> out;
    out.push_back(s);
    int next = route[s];
    while (1) {
        out.push_back(next);
        
        if (next == s) {
            break;
        }
        
        next = route[next];
    }
    reverse(out.begin(), out.end());
    return out;

}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> m;

    for (int i = 1; i <= n; i ++) {
        edges.push_back({0, i, 0});
    }

    for (int i = 0; i < m; i ++) {
        int src, dest, w;
        cin >> src >> dest >> w;
        edges.push_back({src, dest, w});
    }

    process();
    bellmanFord();
    
    

    if (!hasNegCycle) {
        cout << "NO" << endl;
    } else {
        vector<int>&& cycle = getNegCycle();
        
        cout << "YES" << '\n';
        for (int x : cycle) {
            cout << x << " ";
        }
        cout << endl;
    }
}


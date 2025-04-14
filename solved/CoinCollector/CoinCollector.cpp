#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int LEN = 200005;

// use tarjan algorithm
// cool thing about tarjan algorithm is that it finds SCC in reverse topological order
// means when we process SCC i, we already know the result of SCC j, where j is the child of i

template <size_t T>
void printArr(array<int, T> arr) {
    for (int t : arr) cout << t << " ";
    cout << endl;
}

array<ll, LEN> myCoins;
unordered_map<int, ll> maxCoins;

vector<vector<int>> adj;

array<bool, LEN> inStack = {false};
array<bool, LEN> visited = {false};
array<int, LEN> id;
array<int, LEN> low;
vector<int> stac;
int counter = 0;
int sccCnt = 0;

// must return the next available id;
void dfs(int v) {
    stac.push_back(v);
    inStack[v] = true;
    visited[v] = true;
    
    int myId = counter;
    low[v] = counter;
    counter ++;

    for (int c : adj[v]) {
        if (!visited[c]) {
            dfs(c);
        }

        if (inStack[c]) {
            low[v] = min(low[v], low[c]);
        }
    }

    // if I am 'root' of SCC, count max number of coins I can obtain by starting from this SCC
    if (low[v] == myId) {
        ll sum = 0;
        ll bestNext = 0;
        while (1) {
            int u = stac.back(); stac.pop_back();
            inStack[u] = false;
            id[u] = sccCnt;
            sum += myCoins[u];
            
            
            
            // check all child SCC
            for (int c : adj[u]) {
                if (!inStack[c]) {
                    int childSccId = id[c];
                    bestNext = max(bestNext, maxCoins[childSccId]);
                }
            }

            if (u == v) {
                maxCoins[sccCnt] = sum + bestNext;
                sccCnt++;
                break;
            }
        }
    }
}

int main() {
    int N, M;
    cin >> N >> M;

    for (int i = 1; i <= N; i ++) {
        cin >> myCoins[i];
    }

    adj = vector(N + 1, vector<int>());
    for (int i = 0; i < M; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
    }

    
    for (int i = 1; i <= N; i ++) {
        if (visited[i]) continue;
        dfs(i);
    }

   
    ll best = 0;
    for (auto& p : maxCoins) {
        best = max(best, p.second);
    }
    cout << best << endl;
}


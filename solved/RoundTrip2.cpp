#include <bits/stdc++.h>
using namespace std;

// goal: find a cycle of any length (including 2) in a directed graph in O(V + E)
// use kosaraju to find any SCC of size >= 2
// then take any edge in the SCC of a->b, since a, b are in same SCC, there exists a path b to a
// combine path b to a and edge a->b to get your cycle

int N;
vector<vector<int>> adj;
vector<vector<int>> revAdj;
vector<int> sccId; // orginally all -1
vector<int> sccSize;
int src = -1;
int dest = -1;

void processRevEdges() {
    revAdj = vector(N, vector<int>());
    for (int i = 0; i < adj.size(); i ++) {
        for (int j = 0; j < adj[i].size(); j ++) {
            revAdj[adj[i][j]].push_back(i);
        }
    }
}

void dfs1(int v, int vid, vector<int>& stack, vector<int>& visitId) {
    for (int c : adj[v]) {
        if (visitId[c] == -1) {
            visitId[c] = vid;
            dfs1(c, vid, stack, visitId);
        }
    }
    stack.push_back(v);
}

int dfs2(int v, int sccid, int vid, vector<int>& visitId) {
    sccId[v] = sccid;
    int sum = 1;
    
    for (int c : revAdj[v]) {
        if (visitId[c] == vid && sccId[c] == -1) {
            src = c; dest = v; // remember edges are reversed, so it is actually c->v
            sum += dfs2(c, sccid, vid, visitId);
        }
    }
    return sum;
}

void process() {
    int vid = 0;
    int sccid = 0;
    vector<int> visitId = vector(N, -1);
    vector<int> stack; 
    for (int i = 0; i < N; i ++) {
        if (visitId[i] != -1) continue;

        // do the 1st dfs on the original edges
        dfs1(i, vid, stack, visitId);

        while (!stack.empty()) {
            int back = stack.back(); stack.pop_back();
            if (sccId[back] != -1) continue;

            // do 2nd dfs2, on visited edges, on the reversed edges
            int size = dfs2(back, sccid, vid, visitId);
            sccSize.push_back(size);
            sccid ++;
        }
        vid ++;
    }
}

bool dfs3(int v, vector<int>& stack, vector<bool>& visited) {
    visited[v] = true;
    stack.push_back(v);
    if (v == src) {
        stack.push_back(dest);
        return true;
    } else {
        for (int c : adj[v]) {
            if (visited[c]) continue;
            bool ans = dfs3(c, stack, visited);
            if (ans) return true;
        }
        stack.pop_back();
        return false;
    }
}

/*int main() {
    N = 4;
    adj = {{3}, {0,2}, {3}, {}};
    sccId = vector(N, -1);
    processRevEdges();
    process();
    
    for_each(sccId.begin(), sccId.end(), [](int x) -> void {cout << x << " ";});
    cout << endl;
    for_each(sccSize.begin(), sccSize.end(), [](int x) -> void {cout << x << " ";});
    cout << endl;
    cout << src << " " << dest << endl;
}*/

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> N;
    int M;
    cin >> M;
    adj = vector(N, vector<int>());
    sccId = vector(N, -1);

    for (int i = 0; i < M; i ++) {
        int a, b;
        cin >> a >> b;
        adj[a - 1].push_back(b - 1);
    }

    processRevEdges();
    process();

    //cout << src << " " << dest << endl;
    if (src == -1) {
        cout << "IMPOSSIBLE" << "\n";
        return 0;
    }

    vector<int> stack;
    vector<bool> visited(N, false);
    dfs3(dest, stack, visited);
    cout << stack.size() << "\n";
    for (int c : stack) {
        cout << (c + 1) << " ";
    }

}



#include <bits/stdc++.h>
#define MAX 18
using namespace std;

// this is a functional graph, as every vertex has only 1 out edge. We can use binary lifting.
// can go from a to b iff b is the lca of a and b, if lca is different, clearly we cannot reach b from a
// cycles from the 'root', and every vertex will eventually point to a vertex ina a cycle. Once we are in a cycle,
// we cannot go anywhere else, that is why it is a root.
// if we view each cycle as a vertex, the graph forms a tree
// we mark every vertex in a cycle as depth 0

// case 1: both vertices in same cycle, use cycle pos to calculate, remember, n-1 -> ... -> i -> i-1 ... -> 0 -> n - 1
// case 2: both vertices have same cycle as root, dest is in a cycle. Find entry vertex, using getIth, then back to case 1
// case 3: both vertices have same cycle, both not in cycle, depth[src] > depth[dest]. Then lca of src and dest must be dest
// simply check if getIth(src, depth[src] - depth[dest]) == dest

int n;
vector<vector<int>> binLift;
vector<int> outEdge;
vector<int> cycleLen;
vector<int> cycleId; // stores which cycle the vertex is in, or which cycle is the 'root' of that vertex
vector<int> cyclePos; // stores the pos of the vertex in the cycle, or -1 if not
vector<int> depth;
vector<bool> visited;
vector<bool> processed;



void processBinLift() {
    for (int v = 1; v <= n; v ++) {
        binLift[v][0] = outEdge[v];
    }

    for (int i = 1; i <= MAX; i ++) {
        for (int v = 1; v <= n; v ++) {
            binLift[v][i] = binLift[binLift[v][i - 1]][i - 1];
        }
    }
}

int getIth(int src, int i) {
    int pow = 0;
    while (i > 0) {
        if (i & 1) src = binLift[src][pow];
        pow ++;
        i >>= 1;
    }
    return src;
}

void dfs(vector<int>& stack, int v) {
    if (visited[v] && processed[v]) {
        int d = depth[v] + 1;
        int cid = cycleId[v];
        while (!stack.empty()) {
            int n = stack.back(); stack.pop_back();
            processed[n] = true;
            depth[n] = d; d++;
            cycleId[n] = cid;
        }
        
    } else if (visited[v] && !processed[v]) {
        // we are in a cycle;
        int pos = 0;
        int cid = cycleLen.size();
        while (!stack.empty()) {
            int n = stack.back(); stack.pop_back();
            processed[n] = true;
            cyclePos[n] = pos; pos ++;
            cycleId[n] = cid;
            depth[n] = 0;
            if (n == v) break;
        }
        cycleLen.push_back(pos);

        int d = 1;
        while (!stack.empty()) {
            int n = stack.back(); stack.pop_back();
            processed[n] = true;
            cycleId[n] = cid;
            depth[n] = d; d ++;
        }
    } else {
        visited[v] = true;
        stack.push_back(v);
        dfs(stack, outEdge[v]);
    }
}

int query(int src, int dest) {
    if (src == dest) return 0;
    else if (cycleId[src] != cycleId[dest]) return -1;
    else if (depth[src] < depth[dest]) return -1;
    else if (depth[src] == 0 && depth[dest] == 0) {
        int posSrc = cyclePos[src];
        int posDest = cyclePos[dest];
        if (posSrc > posDest) return posSrc - posDest;
        else return posSrc + cycleLen[cycleId[src]] - posDest;
    } else if (depth[dest] == 0) {
        int entry = getIth(src, depth[src]); //cout << entry << endl;
        return depth[src] + query(entry, dest);
    } else {
        // both vertixes have same root, and both not in cycles;
        // then dest must be the lca of src and dest
        return (getIth(src, depth[src] - depth[dest]) == dest) ? depth[src] - depth[dest] : -1;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int N, Q;
    cin >> N >> Q;
    n = N;
    binLift = vector(N + 1, vector(MAX + 1, 0));
    outEdge = vector(N + 1, 0);
    cycleId = vector(N + 1, 0);
    cyclePos = vector(N + 1, 0);
    depth = vector(N + 1, 0);
    visited = vector(N + 1, false);
    processed = vector(N + 1, false);

    for (int i = 1; i <= N; i ++) {
        int out; cin >> out;
        outEdge[i] = out;
    }

    processBinLift();
    vector<int> stack;
    for (int v = 1; v <= N; v ++) {
        if (!visited[v]) dfs(stack, v);
    }

    for (int i = 0; i < Q; i ++) {
        int src, dest; cin >> src >> dest;
        cout << query(src, dest) << "\n";
    }
}









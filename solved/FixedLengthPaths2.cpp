

#include <bits/stdc++.h>
#define LEN 200005
using namespace std;
using ll = long long;

// use centroid decomposition and recursion
// each recursion returns a suffix array of number of nodes at depth 0, 1, 2, starting from recursed node as root
// at every centroid, recurse on each subtree of the centroid. Now we have to find all paths
// of length in [k1, k2] passing and starting at the centroid
// we consider passing through the centroid, i.e path start at vertex in subtree 1, end at vertex in subtree 2
// cannot use prefix array, as merging prefix array is always O(max(len(a), len(b))) (why ? because for the longer array, we have to update
// all the other elem too). For suffix array, we can just insert by processing elements in the new array

// suppose we already have the suffix array for len of paths for subtree containing centroid and subtree 2, starting at centroid
// for path of len x, we need to find sum of number of  paths of len [k1 - 1 - x, k2 - 1 - x], with suffix array this can be done in O(1)
// to merge, suppose we have [a0, a1, a2, a3], where ai = num of paths of len [i, inf], and [b0, b1, b2]
// we simply do [a0 + b0, a1 + b1, a2 + b2, a3] 
// each recursive call returns a suffix array where arr[i] contains path of len in [i, inf] in the subtree, starting from the root of that subtree
// do 1 more dfs to return suffix array with recursed node as root,  NOT CENTROID

// for each call procssing is O(size of subtree) each new recursion call, new size <= size / 2, so T.C is O(nlogn)



int n;
int k1;
int k2;
int weight[LEN];
bool removed[LEN];
vector<int> adj[LEN];
ll ans = 0;


template <typename T>
void print(vector<T> arr) {
    for (T t : arr) {
        cout << t << " ";
    }
    cout << endl;
}

// store for each depth how many nodes, to compute suffix array later
int dfsWeight(int v, int p, unordered_map<int, int>& depths, int d) {
    int sum = 1;
    depths[d] += 1;
    
    for (int c : adj[v]) {
        if (c != p && !removed[c]) sum += dfsWeight(c, v, depths, d + 1);
    }
    
    weight[v] = sum;
    return sum;
}

int findCentroid(int v, int p, int limit) {
    for (int c : adj[v]) {
        if (c != p && !removed[c] && weight[c] > limit / 2) return findCentroid(c, v, limit);
    }
    return v;
}

// start with centroidDecomp(1, 0) not (1, 1)
/*int centroidDecomp(int v, int p) {
    int lim = weight[v];
    int cen = findCentroid(v, p, lim);
    cenWeight[cen] = lim;
    removed[cen] = true;
    
    for (int c : adj[cen]) {
        if (c == p || removed[c]) continue;
        int childCen = centroidDecomp(c, cen);
        cenAdj[cen].push_back(childCen);
    }
    return cen;
}*/


void merge(vector<int>& a, vector<int>& out) {
    int len = a.size();
    out[0] += a[0];
    for (int i = 0; i < len; i ++) {
        out[i + 1] += a[i];
    }
} 

int numOfPaths(int k, vector<int>& suffixArr) {
    int s = max(k1 - k - 1, 0);
    int e = max(k2 - k, 0); // we want path of len [s, e)

    if (s >= suffixArr.size()) return 0;
    // s < suffixArr->size()
    else return suffixArr[s] - ((e >= suffixArr.size()) ? 0 : suffixArr[e]);
}

void processSuffix(unordered_map<int, int>& depths, vector<int>& suf) {
    suf[suf.size() - 1] = depths[suf.size() - 1];
    for (int i = suf.size() - 2; i >= 0; i --) {
        suf[i] = depths[i] + suf[i + 1];
    }
}

vector<int> helper(int v, int p) {
    unordered_map<int, int> depths;
    int lim = dfsWeight(v, p, depths, 0);
    vector<int> out(lim, 0);
    processSuffix(depths, out);

    //if (v == 1) print(*out);

    

    
    vector<int> suffixArr = vector<int>(lim, 0);
    suffixArr[0] = 1;
    int cen = findCentroid(v, p, lim);
    removed[cen] = true;

    for (int c : adj[cen]) {
        if (c == p || removed[c]) continue;
        vector<int>&& pathLen = helper(c, cen);
        
       
       
        int limit =  min(k2 - 1, (int) pathLen.size() - 1);
        for (int i = 0; i <= limit; i ++) {
            ll numPaths = pathLen[i] - ((i == pathLen.size() - 1) ? 0 : pathLen[i + 1]);
            ll possible = numOfPaths(i, suffixArr);
            ans += possible * numPaths;
        }
        merge(pathLen, suffixArr);
        
    }

    return out;

    
}



/*Store* helper(int cen) {
    vector<int>* suffixArr = new vector<int>(cenWeight[cen], 0);
    (*suffixArr)[0] = 1;

    ll ans = 0; // k1 >= 1, so at least 1 edge is needed
    for (int c : cenAdj[cen]) {
        Store* result = helper(c);
        ans += result->res;
        

        vector<int>* pathLen = result->suffixArr;
        if (cen == 7) print(*pathLen);
        for (int i = 0; i <= min(k2 - 1, (int) pathLen->size() - 1); i ++) {
            int numPaths = pathLen->at(i) - ((i == pathLen->size() - 1) ? 0 : pathLen->at(i + 1));
            ans += numPaths * numOfPaths(i, suffixArr);
        }

        
        merge(pathLen, suffixArr);
    }
    //if (cen == 1) print(*suffixArr);
    return new Store(ans, suffixArr);
}*/

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> k1 >> k2;
    
     

    for (int i = 0; i < n - 1; i ++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    
    helper(1, 0);
    
    cout << ans << endl;
}






















/*int main() {
    int len = 13;
    cenAdj = vector(len, vector<int>());
    adj = {{}, {2},{1,3},{2,4,5,6},{3},{3,7,8,10},{3,9},{5},{5},{6,11,12},{5},{9},{9}};
    centroidDecomp(1, 0);
    for (int i = 1; i < len; i ++) {
        cout << i << ": ";
        print(cenAdj[i]);
    }
}*/


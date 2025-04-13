
#include <bits/stdc++.h>
using namespace std;
#define LEN 200005

// DONT USE set to compress, very slow

// part 1: for each range [s, e], find how many ranges it contains
// sort in ascending s, then descending e (IMPT, otherwise, we will miss some ranges)
// starting from the back, we input e into segtree one by one. Hence, when we process [si, ei], all ranges in segtree have
// sj >= si, hence we only need to count all ej <= ei
 
// part 2: for each [si, ei] find how many ranges contains it
// sort in ascending e, then descending s
// starting from the back, we input si into the segtree one by one. When we process [si, ei], all the sj in segtree
// have ej >= ei. Count of all sj <= si is number of ranges containing [si, ei]
 
struct SegTree {
    vector<int> tree;
    int n;
 
    SegTree(int n): tree(4 * n, 0), n(n) {}
 
    void update(int p, int s, int e, int idx, int val) {
        if (s == e) {
            tree[p] += val;
            return;
        }
        
        int m = (s + e) >> 1;
        if (idx <= m) update((p << 1) + 1, s, m , idx, val);
        else if (idx > m) update((p << 1) + 2, m + 1, e, idx, val);
        tree[p] = tree[(p << 1) + 1] + tree[(p << 1) + 2];
    }
 
    int query(int p, int s, int e, int l, int r) {
        if (s == e || (s == l && e == r)) return tree[p];
        
        int m = (s + e) >> 1;
        if (r <= m) return query((p << 1) + 1, s, m, l, r);
        else if (l > m) return query((p << 1) + 2, m + 1, e, l, r);
        else return query((p << 1) + 1, s, m, l, m) + query((p << 1) + 2, m + 1, e, m + 1, r);
    }
};
 
int n;
unordered_map<int, int> compressed;
int ans1[LEN]; // how many ranges it contains
int ans2[LEN]; // how many ranges contains it
vector<vector<int>> Ranges; // [s1, e1, idx]
 
void compress() {
    vector<int> s;
    for (vector<int>& r : Ranges) {
        if (compressed.find(r[0]) == compressed.end()) {
            s.push_back(r[0]);
            compressed[r[0]] = 1;
        }

        if (compressed.find(r[1]) == compressed.end()) {
            s.push_back(r[1]);
            compressed[r[1]] = 1;
        }
    }
    sort(s.begin(), s.end());
    n = s.size();
 
    int i = 0;
    for (auto it = s.begin(); it != s.end(); it ++) {
        compressed[*it] = i;
        i ++;
    }
}
 
void processAns1() {
    auto cmp = [] (vector<int>& a, vector<int>& b) -> bool {return (a[0] != b[0]) ? (a[0] < b[0]) : b[1] < a[1];};
    sort(Ranges.begin(), Ranges.end(), cmp);
    SegTree st(n);
    
    for (auto it = Ranges.rbegin(); it != Ranges.rend(); it ++) {
        int si = (*it)[0];
        int ei = (*it)[1];
        int idx = (*it)[2];
        
        int cnt = st.query(0, 0, st.n - 1, 0, compressed[ei]);
        ans1[idx] = cnt;
        st.update(0, 0, st.n - 1, compressed[ei], 1);
    }
}
 
void processAns2() {
    auto cmp = [] (vector<int>& a, vector<int>& b) -> bool {return (a[1] != b[1]) ? (a[1] < b[1]) : b[0] < a[0];};
    sort(Ranges.begin(), Ranges.end(), cmp);
    SegTree st(n);
 
    for (auto it = Ranges.rbegin(); it != Ranges.rend(); it ++) {
        int si = (*it)[0];
        int ei = (*it)[1];
        int idx = (*it)[2];
        
        int cnt = st.query(0, 0, st.n - 1, 0, compressed[si]);
        ans2[idx] = cnt;
        st.update(0, 0, st.n - 1, compressed[si], 1);
    }
}
 
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int N;
    cin >> N;
    //ans1 = vector(N, 0);
    //ans2 = vector(N, 0);
    
    for (int i = 0; i < N; i ++) {
        int s, e;
        cin >> s >> e;
        Ranges.push_back({s, e, i});
    }
 
    compress();
    /*for (const auto& [key, value] : compressed) {
        cout << key << " " << value << endl;
    }*/
    
    processAns1();
    processAns2();
    
    for (int i = 0; i < N; i ++) {
        cout << ans1[i] << " ";
    }
    cout << "\n";
 
    
    for (int i = 0; i < N; i ++) {
        cout << ans2[i] << " ";
    }
    cout << "\n";
 
    
}
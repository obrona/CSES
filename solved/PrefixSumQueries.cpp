#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// max prefix sum of [s, e] = max(max prefix sum [s, m], sum [s, m] + max prefix sum [m + 1, e])

struct Pair {
    ll a, b;
    Pair(ll a, ll b): a(a), b(b) {}
};

class SegTree {
public:
    vector<Pair> tree;
    int n;

    SegTree(vector<int>& arr): n(arr.size()) {
        tree = vector(4 * n, Pair(0, 0));
        build(0, 0, n - 1, arr);
    }

    void build(int p, int s, int e, vector<int>& arr) {
        if (s == e) {
            tree[p].a = arr[s];
            tree[p].b = arr[s];
        } else {
            int m = (s + e) / 2;
            build(2 * p + 1, s, m, arr);
            build(2 * p + 2, m + 1, e, arr);
            tree[p].a = max(tree[2 * p + 1].a, tree[2 * p + 1].b + tree[2 * p + 2].a);
            tree[p].b = tree[2 * p + 1].b + tree[2 * p + 2].b;
        }
    }

    void update(int p, int s, int e, int idx, int val) {
        int m = (s + e) / 2;
        if (s == e) {
            tree[p].a = val;
            tree[p].b = val;
            return;
        } 
        
        if (idx <= m) {
            update(2 * p + 1, s, m, idx, val);
        } else {
            update(2 * p + 2, m + 1, e, idx, val);
        }
        tree[p].a = max(tree[2 * p + 1].a, tree[2 * p + 1].b + tree[2 * p + 2].a);
        tree[p].b = tree[2 * p + 1].b + tree[2 * p + 2].b;
    }

    Pair query(int p, int s, int e, int l, int r) {
        int m = (s + e) / 2;
        if (s == e || (s == l && e == r)) {
            return tree[p];
        } else if (r <= m) {
            return query(2 * p + 1, s, m, l, r);
        } else if (l > m) {
            return query(2 * p + 2, m + 1, e, l, r);
        } else {
            Pair left = query(2 * p + 1, s, m, l, m);
            Pair right = query(2 * p + 2, m + 1, e, m + 1, r);
            return Pair(max(left.a, left.b + right.a), left.b + right.b);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    vector<int> arr;
    for (int i = 0; i < n; i ++) {
        int x;
        cin >> x;
        arr.push_back(x);
    }

    SegTree st(arr);
    for (int i = 0; i < q; i ++) {
        int t;
        cin >> t;
        if (t == 1) {
            int idx, val;
            cin >> idx >> val;
            st.update(0, 0, st.n - 1, idx - 1, val);
        } else {
            int s, e;
            cin >> s >> e;
            ll ans = st.query(0, 0, st.n - 1, s - 1, e - 1).a;
            cout << ((ans < 0) ? 0 : ans) << "\n"; 
        }
    }
}
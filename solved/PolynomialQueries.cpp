#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// lazy[p], for range [l, r] stores array(si), where si means we add si to elem l, si + 1 to elem l + 1 and so on
// when we go to child node, we need this info
// when we need the result of child node [s, e]
// len: len of child node
// v: start index, i.e s
// vi: start index of parent node
// we need len * (sum(si + v - vi + si + v - vi + len - 1)) 
// doing some math, we only need to store sum(si) and sum(vi) and number of such queries ending above this node, denoted as k

struct Pair {
    ll p0, p1;
    Pair(ll x, ll y): p0(x), p1(y) {};
};





class SegTree {
public:
    int sz;
    vector<ll> tree;
    vector<Pair> lazy;

    SegTree(vector<ll>& arr): sz(arr.size()), tree(4 * arr.size(), 0), lazy(4 * arr.size(), Pair(0, 0)) {
        build(0, 0, sz - 1, arr);
    }

    void build(int p, int s, int e, vector<ll>& arr) {
        if (s == e) {
            tree[p] = arr[s];
            return;
        }
        int m = (s + e) >> 1;
        build((p << 1) + 1, s, m, arr);
        build((p << 1) + 2, m + 1, e, arr);
        tree[p] = tree[(p << 1) + 1] + tree[(p << 1) + 2];
    }

    // processes the offset, i.e how much to add to tree[p] tp find sum of range [s, e]
    ll process(ll k, ll si, ll vi, int s, int e) {
        ll len = e - s + 1;
        return (len * (2 * si - 2 * vi + k * len + 2 * k * s - k)) >> 1;
    }

    // vi is taken implicitly from s of [s, e]
    ll query(int p, int s, int e, int l, int r, ll si, ll vi, ll k) {
        if (s == e || (s == l && e == r)) {
            return tree[p] + process(k, si, vi, s, e);
        }

        int m = (s + e) >> 1;

        // if cnt == 0, no change to si, vi, k
        ll sum = lazy[p].p0;
        ll cnt = lazy[p].p1;
        if (r <= m) {
            return  query((p << 1) + 1, s, m, l, r, si + sum, vi + s * cnt, k + cnt);
        } else if (l > m) {
           return query((p << 1) + 2, m + 1, e, l, r, si + sum, vi + s * cnt, k + cnt);
        } else {
            return query((p << 1) + 1, s, m, l, m, si + sum, vi + s * cnt, k + cnt)
                + query((p << 1) + 2, m + 1, e, m + 1, r, si + sum, vi + s * cnt, k + cnt);
        }
    }


    // this means you add start to elem l, start + 1 to elem l + 1, start + 2 to elem l + 2 ...
    ll update(int p, int s, int e, int l, int r, int start, ll si, ll vi, ll k) {
        ll offset = process(k, si, vi, s, e);
        if (s == e || (s == l && e == r)) {
            ll len = e - s + 1;
            ll actualSum = ((len * (start + start + len - 1)) >> 1) + tree[p] + offset;
            tree[p] = actualSum - offset;
            lazy[p].p0 += start;
            lazy[p].p1 += 1;
            return actualSum;
        }

        int m = (s + e) >> 1;
        ll sum = lazy[p].p0;
        ll cnt = lazy[p].p1;
        if (r <= m) {
           ll lsum = update((p << 1) + 1, s, m, l, r, start, si + sum, vi + s * cnt, k + cnt);
            
            ll offsetr = process(k + cnt, si + sum, vi + s * cnt, m + 1, e);
            ll rsum = offsetr + tree[(p << 1) + 2];
            tree[p] = lsum + rsum - offset;
            return lsum + rsum;
        } else if (l > m) {
            ll offsetl = process(k + cnt, si + sum, vi + s * cnt, s, m);
            ll lsum = offsetl + tree[(p << 1) + 1];
            
            ll rsum = update((p << 1) + 2, m + 1, e, l, r, start, si + sum, vi + s * cnt, k + cnt);
            tree[p] = lsum + rsum - offset;
            return lsum + rsum;
        } else {
            ll lsum = update((p << 1) + 1, s, m, l, m, start, si + sum, vi + s * cnt, k + cnt);
            ll rsum = update((p << 1) + 2, m + 1, e, m + 1, r, start + m + 1 - l, si + sum, vi + s * cnt, k + cnt);
            tree[p] = lsum + rsum - offset;
            return lsum + rsum;
        }
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, q;
    cin >> n >> q;

    vector<ll> arr;
    for (int i = 0; i < n; i ++) {
        ll x;
        cin >> x;
        arr.push_back(x);
    }

    SegTree st(arr);
    for (int i = 0; i < q; i ++) {
        int t, l, r;
        cin >> t >> l >> r;
        if (t == 1) {
            st.update(0, 0, st.sz - 1, l - 1, r - 1, 1, 0, 0, 0);
        } else {
            cout << st.query(0, 0, st.sz - 1, l - 1, r - 1, 0, 0, 0) << "\n";
        }

    }
}
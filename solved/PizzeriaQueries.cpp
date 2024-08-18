#include <bits/stdc++.h>
using namespace std;
#define LEN 200005

// for this we break down into 2 queries, query to all pizzerias on the left, what is the min cost, and do the same for the right

// for left tree, tree[p] contains the min price of pizza delivery, in range [s, e] taking the destination as  e
class LeftTree {
public:
    int tree[4 * LEN];
    int n;
    
    LeftTree(vector<int>& pizzas): n(pizzas.size()) {
        build(0, 0, n - 1, pizzas);
    }

    void build(int p, int s, int e, vector<int>& pizzas) {
        if (s == e) {
            tree[p] = pizzas[s];
            return;
        }

        int m = (s + e) / 2;
        int offset = e - m;
        build(2 * p + 1, s, m, pizzas);
        build(2 * p + 2, m + 1, e, pizzas);
        tree[p] = min(offset + tree[2 * p + 1], tree[2 * p + 2]);
    }

    void update(int p, int s, int e, int idx, int val) {
        if (s == e) {
            tree[p] = val;
            return;
        }

        int m = (s + e) / 2;
        int offset = e - m;
        if (idx <= m) update(2 * p + 1, s, m, idx, val);
        else update(2 * p + 2, m + 1, e, idx, val);
        tree[p] = min(offset + tree[2 * p + 1], tree[2 * p + 2]);
    }

    // invariant s <= idx i.e since this is left query only, we never consider ranges to the right of idx
    // returns for pizzas in range [s, e] the cheapest pizza, including the delivery cost of |idx - i|
    int query(int p, int s, int e, int idx) {
        if (s == e || idx >= e) return tree[p] + idx - e; // very important, if not query T.C is O(n) as we search both sides when we split
       

        int m = (s + e) / 2;
        if (idx <= m) {
            return query(2 * p + 1, s, m, idx);
        } else {
            int left = query(2 * p + 1, s, m, idx);
            int right = query(2 * p + 2, m + 1, e, idx);
            return min(left, right);
        }
    }

};

// for right tree, tree[p] contains the min price of pizza delivery, in range [s, e] taking the destination as s
// so s <= idx
class RightTree {
public:
    int tree[4 * LEN];
    int n;

    RightTree(vector<int>& pizzas): n(pizzas.size()) {
        build(0, 0, n - 1, pizzas);
    }

    void build(int p, int s, int e, vector<int>& pizzas) {
        if (s == e) {
            tree[p] = pizzas[s];
            return;
        }

        int m = (s + e) / 2;
        int offset = m - s + 1;
        build(2 * p + 1, s, m, pizzas);
        build(2 * p + 2, m + 1, e, pizzas);
        tree[p] = min(tree[2 * p + 1], tree[2 * p + 2] + offset);
        
    }

    // here idx <= e  as it returns in range [s, e] the cheapest pizza to the right and including idx
    void update(int p, int s, int e, int idx, int val) {
        if (s == e) {
            tree[p] = val;
            return;
        }

        int m = (s + e) / 2;
        int offset = m - s + 1;
        if (idx <= m) update(2 * p + 1, s, m, idx, val);
        else update(2 * p + 2, m + 1, e, idx, val);
        tree[p] = min(tree[2 * p + 1], offset + tree[2 * p + 2]);
    }

    // idx <= e also, as it idx > e, [s, e] is never included in the overall query
    int query(int p, int s, int e, int idx) {
        if (s == e || idx <= s) {
            return tree[p] + s - idx;
        } 

        int m = (s + e) / 2;
        int offset = m - s + 1;
        if (idx > m) {
            return query(2 * p + 2, m + 1, e, idx);
        } else {
            return min(query(2 * p + 1, s, m, idx), query(2 * p + 2, m + 1, e, idx));
        }
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n, q;
    cin >> n >> q;
   
    vector<int> pizzas;
    for (int i = 0; i < n; i ++) {
        int p;
        cin >> p;
        pizzas.push_back(p);
    }

    LeftTree ltree(pizzas);
    RightTree rtree(pizzas);

    for (int i = 0; i < q; i ++) {
        int t;
        cin >> t;
        if (t == 1) {
            int k, x;
            cin >> k >> x;
            ltree.update(0, 0, n - 1, k - 1, x);
            rtree.update(0, 0, n - 1, k - 1, x);
        } else {
            int k;
            cin >> k;
            int l = ltree.query(0, 0, n - 1, k - 1);
            int r = rtree.query(0, 0, n - 1, k - 1);
            cout << min(l, r) << '\n';
        }
    }

}
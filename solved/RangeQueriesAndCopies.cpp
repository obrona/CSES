#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// persistant segment trees

struct Node {
    ll sum;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(ll sum): sum(sum) {}

    Node(Node *left, Node *right): sum(sum), left(left), right(right) {
        sum = left->sum + right->sum;
    }

    static Node *build(int s, int e, vector<int>& arr) {
        if (s == e) return new Node(arr[s]);

        int m = (s + e) / 2;
        return new Node(build(s, m, arr), build(m + 1, e, arr));
    }

    

    static Node *update(Node *curr, int s, int e, int idx, int val) {
        Node *node = new Node(val);
        if (s == e) return node;

        int m = (s + e) / 2;
        if (idx <= m) {
            node->left = update(curr->left, s, m, idx, val);
            node->right = curr->right;
        } else {
            node->left = curr->left;
            node->right = update(curr->right, m + 1, e, idx, val);
        }
        node->sum = node->left->sum + node->right->sum;
        return node;
    }

    static ll query(Node* curr, int s, int e, int l, int r) {
        if (s == e || (s == l && e == r)) return curr->sum;

        int m = (s + e) / 2;
        if (r <= m) return query(curr->left, s, m, l, r);
        else if (l > m) return query(curr->right, m + 1, e, l, r);
        else return query(curr->left, s, m, l, m) + query(curr->right, m + 1, e, m + 1, r);
    }
};

vector<Node*> segtrees;
vector<int> arr;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int N, Q;
    cin >> N >> Q;
    
    for (int i = 0; i < N; i ++) {
        int x;
        cin >> x;
        arr.push_back(x);
    }

    segtrees.push_back(Node::build(0, N - 1, arr));

    for (int i = 0; i < Q; i ++) {
        int t;
        cin >> t;
        if (t == 1) {
            int k, a, x;
            cin >> k >> a >> x;
            // create a "new" segment tree at idx k - 1
            segtrees[k - 1] = Node::update(segtrees[k - 1], 0, N - 1, a - 1, x);
        } else if (t == 2) {
            int k, a, b;
            cin >> k >> a >> b;
            cout << Node::query(segtrees[k - 1], 0, N - 1, a - 1, b - 1) << "\n";
        } else {
            int k;
            cin >> k;
            segtrees.push_back(segtrees[k - 1]);
        }
    }
}









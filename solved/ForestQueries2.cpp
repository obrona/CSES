#include <bits/stdc++.h>
using namespace std;

// 2d segment tree
// learnt something new, order of member initliaser list is from, the order the members are defined in the class !!, not 
// how they were defined int the member initiliaser list. see https://learn.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=msvc-170

// clarification: y is the 1st indice, x is the 2nd indice of arr
// so (y, x) refers to arr[y][x] i.e y refers to the list idx, x refers to the idx of the elem in the list
// in segtree the 2 letters are switched, x refers to the list idx, y refers to the idx of the elem in the list

class SegTree {
public:
    int nx;
    int ny;
    vector<vector<int>> tree;

    SegTree(vector<vector<int>>& arr): nx(arr.size()), ny(arr.size()), tree(4 * nx, vector<int>(4 * ny, 0)) {
        build(0, 0, nx - 1, arr);
    }

    // x0, xl, xr, are all the pointer to the position
    // we only call this function once we build the left and right segment trees
    void merge(int px, int pxl, int pxr, int py, int sy, int ey) {
        int m = (sy + ey) / 2;
        tree[px][py] = tree[pxl][py] + tree[pxr][py];
        if (sy != ey) {
            merge(px, pxl, pxr, 2 * py + 1, sy, m);
            merge(px, pxl, pxr, 2 * py + 2, m + 1, ey);
        }
    }

    // builds the segment tree when sx == ex
    void buildY(int x, int px, int py, int sy, int ey, vector<vector<int>>& arr) {
        int m = (sy + ey) / 2;
        if (sy == ey) tree[px][py] = arr[x][sy];
        else {
            buildY(x, px, 2 * py + 1, sy, m, arr);
            buildY(x, px, 2 * py + 2, m + 1, ey, arr);
            tree[px][py] = tree[px][2 * py + 1] + tree[px][2 * py + 2];
        }
    }
    
    void build(int px, int sx, int ex, vector<vector<int>>& arr) {
        int m = (sx + ex) / 2;
        if (sx == ex) {
            buildY(sx, px, 0, 0, ny - 1, arr);
        } else {
            build(2 * px + 1, sx, m, arr);
            build(2 * px + 2, m + 1, ex, arr);
            merge(px, 2 * px + 1, 2 * px + 2, 0, 0, ny - 1);
        }
    }


    
    
    int queryY(int px, int py, int sy, int ey, int ly, int ry) {
        int m = (sy + ey) / 2;
        if (sy == ey || (sy == ly && ey == ry)) return tree[px][py];
        else if (ry <= m) {
            return queryY(px, 2 * py + 1, sy, m, ly, ry);
        } else if (ly > m) {
            return queryY(px, 2 * py + 2, m + 1, ey, ly, ry);
        } else {
            return queryY(px, 2 * py + 1, sy, m, ly, m) + queryY(px, 2 * py + 2, m + 1, ey, m + 1, ry);
        }
    }

    int query(int px, int sx, int ex, int lx, int rx, int ly, int ry) {
        int m = (sx + ex) / 2;
        if (sx == ex || (sx == lx && ex == rx)) {
            return queryY(px, 0, 0, ny - 1, ly, ry);
        } else if (rx <= m) {
            return query(2 * px + 1, sx, m, lx, rx, ly, ry);
        } else if (lx > m) {
            return query(2 * px + 2, m + 1, ex, lx, rx, ly, ry);
        } else {
            return query(2 * px + 1, sx, m, lx, m, ly, ry) + query(2 * px + 2, m + 1, ex, m + 1, rx, ly, ry);
        }
    }

    void mergeUpdate(int px, int pxl, int pxr, int py, int sy, int ey, int y) {
        tree[px][py] = tree[pxl][py] + tree[pxr][py];
        int m = (sy + ey) / 2;
        if (sy == ey) {
            return;
        } else if (y <= m) {
            mergeUpdate(px, pxl, pxr, 2 * py + 1, sy, m, y);
        } else {
            mergeUpdate(px, pxl, pxr, 2 * py + 2, m + 1, ey, y);
        }
    }

    void updateY(int px, int py, int sy, int ey, int idx, int val) {
        if (sy == ey) {
            tree[px][py] = val;
            return;
        }
        
        int m = (sy + ey) / 2;
        if (idx <= m) updateY(px, 2 * py + 1, sy, m, idx, val);
        else if (idx > m) updateY(px, 2 * py + 2, m + 1, ey, idx, val);
        tree[px][py] = tree[px][2 * py + 1] + tree[px][2 * py + 2];
       
    }

    void update(int px, int sx, int ex, int x, int y, int val) {
        if (sx == ex) {
            updateY(px, 0, 0, ny - 1, y, val);
            return;
        }
        
        int m = (sx + ex) / 2;
        if (x <= m) {
            update(2 * px + 1, sx, m, x, y, val);
        } else {
            update(2 * px + 2, m + 1, ex, x, y, val);
        }
        mergeUpdate(px, 2 * px + 1, 2 * px + 2, 0, 0, ny - 1, y);
    }
    

};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int X, Q;
    cin >> X >> Q;
    vector<vector<int>> arr(X, vector(X, 0));
    for (int i = 0; i < X; i ++) {
        for (int j = 0; j < X; j ++) {
            char c;
            cin >> c;
            if (c == '*') arr[i][j] = 1;
        }
    }

    SegTree st(arr);
    for (int i = 0; i < Q; i ++) {
        int t;
        cin >> t;
        if (t == 1) {
            int x, y;
            cin >> x >> y;
            x --; y --;
            if (arr[x][y]) {
                arr[x][y] = 0;
                st.update(0, 0, st.nx - 1, x, y, 0);
            } else {
                arr[x][y] = 1;
                st.update(0, 0, st.nx - 1, x, y, 1);
            }
        } else {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            x1 --; y1 --; x2 --; y2 --;
            cout << st.query(0, 0, st.nx - 1, x1, x2, y1, y2) << "\n";
        }
    }
    
}
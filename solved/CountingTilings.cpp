#include <bits/stdc++.h>
using namespace std;
#define MOD 1000000007

// each state is (bits, lvl) bit is 1 if a block at lvl i needs to be covered, 0 if it is already covered by next level
// eg 011010
// for each state, for each uncovered block, we can place a 1 * 2 tile, in which case, the block underneath is covered, so 
// no change to bottom
// or place a 2 * 1 tile if possible, so bottom += (1 << idx) + (1 << (idx + 1))
// if block is already covered, underneath block will be uncovered, so bottom += (1 << idx)

// states is 2^n * m
// transition between each state is O(2^n) but I still get accepted


vector<vector<int>> dp;
int n, m;

int helper(int bits, int lvl, int idx, int bottom) {
    if (idx == n) {
        if (lvl == 0) {
            // for lvl 0, bottom must all be 1, because we cannot have a 1 * 2 tile on lvl 0
            return bottom == (1 << n) - 1;
        } else {
            return dp[bottom][lvl - 1];
        }
    } 
    
    int sum = 0;
    if ((bits >> idx) & 1) {
        // place 2 * 1
        if (idx < n - 1 && (bits >> (idx + 1) & 1)) {
            sum += helper(bits, lvl, idx + 2, bottom + (1 << idx) + (1 << (idx + 1)));
            sum %= MOD;
        }

        // place a 1 * 2, bottom tile mark as 0, no need to add to bottom
        sum += helper(bits, lvl, idx + 1, bottom);
        sum %= MOD;
        return sum;
    } else {
        // need to add (1 << idx), as the bottom is uncovered
        return helper(bits, lvl, idx + 1, bottom + (1 << idx));
    }
}

void processDp() {
    int size = (1 << n);
    for (int lvl = 0; lvl < m; lvl ++) {
        for (int bits = 0; bits < size; bits ++) {
            dp[bits][lvl] = helper(bits, lvl, 0, 0);

        }
    }
}

int main() {
    cin >> n >> m;
    dp = vector(1 << n, vector(m, 0));
    processDp();
    //cout << helper(9, 0, 0, 0) << endl;
    cout << dp[(1 << n) - 1][m - 1] << endl;

}
#include <bits/stdc++.h>
#define MOD_INV 500000004
#define MOD 1000000007

// let state (diff, i) be how many ways to make 2 equal sets from num 1,2,3...i with the sum(set1) - sum(set2) = diff
// putting num1 in set1 or set2 is different, so the sum of possible sets will be multiplied by 2, as (set1, set2) is different from (set2, set1)
// so we must do modulo inverse i.e num of (ways calculated/2) mod 1000000007 as we have to divide the actual calculated number of ways
// to partition into 2 sets where sum is equal where order of set matters by 2, but we only know the modulo

using namespace std;
using ll = long long;

vector<vector<int>> dp;
int n;
int offset;

ll process() {
    for (int diff= -offset; diff <= offset; diff ++) {
        dp[0][diff + offset] = (abs(diff) == 1);
    }

    //state(idx, diff) is stored as (i, diff + offset)
    for (int i = 2; i <= n; i ++) {
        for (int diff = -offset; diff <= offset; diff ++) {
            
            // diff + i and diff - i must still be a valid set difference, so abs(diff) <= n(n + 1)/2, i.e when all nums are in 1 set
            int ans = 0;
            ans += (diff + i > offset) ? 0 : dp[i - 2][diff + i + offset];
            ans %= MOD;
            ans += (diff - i < -offset) ? 0 : dp[i - 2][diff - i + offset];
            ans %= MOD;
            dp[i - 1][diff + offset] = ans;
        }
    }

    
    ll res = dp[n - 1][offset];
    res *= MOD_INV;
    res %= MOD;
    return res;
}

int main() {
    cin >> n;
    offset = n * (n + 1) / 2;
    dp = vector(n, vector(2 * offset + 1, 0));
    cout << process();
}


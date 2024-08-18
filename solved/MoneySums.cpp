#include <bits/stdc++.h>
using namespace std;

// simple dp, let dp(i, m) = possible to create momey m with coin0, coin1 ... coin(i-1)
// total is 10^7, still can I guess

vector<vector<bool>> dp;
vector<int> coins;
//vector<bool> possible;
int LEN;


void helper() {
    // idx == 0
    for (int m = 0; m <= 100000; m ++) {
        dp[0][m] = (m == 0 || coins[0] == m);
    }
    for (int i = 1; i < LEN; i ++) {
        for (int m = 0; m <= 100000; m ++) {
            //cout << m << " " << i << endl;
            bool ans = false;
            if (dp[i - 1][m]) {
                ans = true;
            } else if (m - coins[i] >= 0 && dp[i - 1][m - coins[i]]){
                ans = true;
            }
            dp[i][m] = ans;
        }
    }
}

void printResult() {
    helper();
    vector<int> possible;
    for (int m = 1; m <= 100000; m ++) {
        for (int i = 0; i < LEN; i ++) {
            if (dp[i][m]) {
                possible.push_back(m);
                break;
            }
            
        }
    }
    cout << possible.size() << "\n";
    for_each(possible.begin(), possible.end(), [](int x) -> void {cout << x << " ";});
}

int main() {
    cin >> LEN;
    //cout << LEN << endl;
    for (int i = 0; i < LEN; i ++) {
        int coin;
        cin >> coin;
        coins.push_back(coin);
    }
    //for_each(coins.begin(), coins.end(), [](int c) -> void {cout << c << " ";});
    dp = vector(LEN, vector(100001, false));
    printResult();

}




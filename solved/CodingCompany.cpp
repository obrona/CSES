#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MOD 1000000007

// 1st sort the skill in ascending order
// open closed interval dp
// let dp[i][j][k] mean how many ways to "group" prefix[0..i] such that there are j "open interval", with k score
// open interval, means this group has not yet recieved the highest skilled coder
// for dp[i][j][k] we have 4 options for the new ith person
// 1. put ith person in any of the j open intervals, but not as the best skilled coder
// 2. make the ith person start a new open interval, i.e this person is the least skilled
// 3. make the ith person close a interval i.e this person is the most skilled in the group he is assigned too. We have j + 1 choices
// 4. make the ith person a 1 man group
// if this coder is some group best skilled, score + skill[i]
// if this code is some group worst skilled, score - skill[i]
// 
// 1. dp(i - 1, j, k) * j
// 2. dp(i - 1, j - 1, k + skill[i])
// 3. dp(i - 1, j + 1, k - skill[i]) * j
// 4. dp(i - 1, k, k)
//
// time limit very tight, need to resubmit a few times to get accepted
// try to reduce modulo ops as much as possible, use ll to store large values, then modulo final result

int n;
int limit;
int skills[100];
bool visited[100][101][10001];
int dp[100][101][10001];

int offset(int k) {
    return k + 5000;
}

ll modMult(int a, int b) {
    ll res = (ll) a * b;
    
    return res;
}

int helper(int i, int j, int k) {
    if (k < -5000 || k > 5000) return 0;
    if (j > i + 1 || j < 0) return 0;
    
    if (visited[i][j][k + 5000]) return dp[i][j][k + 5000];
    
    if (i == 0) {
        if (j == 0) {
            dp[i][j][k + 5000] = (k == 0) ? 1 : 0;
        } else if (j == 1) {
           dp[i][j][k + 5000] = (k  == -skills[i]) ? 1 : 0;
        } else {
            dp[i][j][k + 5000] = 0;
        }
    } else {
        ll opts[4] = {modMult(helper(i - 1, j, k), j), helper(i - 1, j - 1, k + skills[i]), 
            modMult(helper(i - 1, j + 1, k - skills[i]), j + 1), helper(i - 1, j, k)};
        ll res = accumulate(opts, opts + 4, 0LL, [](ll accu, ll ans) -> ll {return accu + ans;});
        dp[i][j][k + 5000] = res % MOD;
    }
    visited[i][j][k + 5000] = true;
    return dp[i][j][k + 5000];
}




int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> limit;
    
    for (int i = 0; i < n; i ++) {
        int skill;
        cin >> skill;
        skills[i] = skill;
    }
    sort(skills, skills + n);
    /*for (int i = 0; i < n; i ++) {
        cout << skills[i] << endl;
    }*/

    //clear();
    
    int sum = 0;
    for (int s = 0; s <= limit; s ++) {
        int ans = helper(n - 1, 0, s);
        sum += ans;
        sum %= MOD;
        //cout << ans << endl;
    }
    
    
    cout << sum << endl;
    
    
}

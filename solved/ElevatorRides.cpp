#include <bits/stdc++.h>
using namespace std;

// the naive algo where for each set of people, we take a non-empty subset then recurse will TLE as T.C is O(3**n) (why?)

// Solution is using greedy + dp
// for each set, there are multiple ways to group the people, but we want the grouping, such that the last group sum 
// is minimised
// now for X, a set of people, there must be at least 1 person x, from X, in the last group. This can be p1, p2, p3 ...
// consider case i, where pi is confirmed to be in the last group
// we only need to consider the smallest last group sum of X - pi, call it G
// if G + pi <= limit, then G + pi is the smallest last group sum with pi in last group, otherwise, G will not be the 
// smallest last group sum of X - pi
// if not, smallest last group sum with pi in last group is pi itself

int N;
int weights[20];
int lastSum[1 << 20];
int dp[1 << 20];
int limit;

int process() {
    int sets = 1 << N;
    dp[0] = 1;
    lastSum[0] = 0;
    for (int i = 1; i < sets; i ++) {
        int numGroups = 9999;
        int lastsum = 1000000009;
        for (int j = 0; j < N; j ++) {
            if (!((i >> j) & 1)) continue;
            int left = i ^ (1 << j);
            int last = lastSum[left];
            int num = dp[left];

            int reslast = (last + weights[j] > limit) ? weights[j] : last + weights[j];
            int resnum = (last + weights[j] > limit) ? num + 1 : num;

            if (resnum > numGroups) continue;
            else if (resnum < numGroups) {
                numGroups = resnum;
                lastsum = reslast;
            } else if (reslast < lastsum) {
                lastsum = reslast;
            }
            
        }
        dp[i] = numGroups;
        lastSum[i] = lastsum;
        
    }
    return dp[sets - 1];

}

int main() {
    cin >> N >> limit;
    for (int i = 0; i < N; i ++) {
        int w;
        cin >> w;
        weights[i] = w;
    }
    cout << process();
}


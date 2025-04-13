#include <bits/stdc++.h>
using namespace std;

// we sort the intervals, in increasing end time and decreasing start time
// when we have a new range, we give it to the person with the closest end time, i.e. the person with the largest end time
// that is still lesser than the range start time
// Pf. Suppose we dont need to and still get optimal solution. The person, A that gets that range has an end time lesser than the former person, B
// all subsequent ranges of A, give to B, and all subsequent ranges of B, give to A

// we always consider the ranges with the earliest end time and earlies start time 1st, call it R
// Suppose we do not use R. Then the range that replaces R definitely has end time >= R.endtime
// regardless of whether that range start time is lesser or greater than R, we can swap 

int n, k;
set<int> pq;
unordered_map<int, int> freq;

auto cmp = [](vector<int>& a, vector<int>& b) {return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]);};
vector<vector<int>> movies;

set<int>::iterator biggestElemLesserThan(int x, set<int>& s) {
    if (s.size() == 0) return s.end();

    auto it = s.lower_bound(x);
    if (it == s.end()) return prev(it);
    else if (*it > x) {
        if (it == pq.begin()) return s.end();
        else return prev(it);
    } else return it;
}

int helper() {
    sort(movies.begin(), movies.end(), cmp);
    for (vector<int> m : movies) {
        //cout << m[0] << " " << m[1] << endl;
    }
    
    int people = k;
    int cnt = 0;
    int ptr = 0;
    

    while (ptr < movies.size()) {
        int s = movies[ptr][0];
        int e = movies[ptr][1];
        
        auto it = biggestElemLesserThan(s, pq);
        if (it != pq.end()) {
            //cout << "yes" << endl;
            freq[*it] --;
            if (freq[*it] == 0) pq.erase(*it);
            people ++;
        }
        

        if (people > 0) {
            //cout << s << " " << e << endl;
            people --;
            if (freq[e] == 0) pq.insert(e);
            freq[e] ++;
            cnt ++;
        }
        ptr ++;
    }
    return cnt;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> k;

    for (int i = 0; i < n; i ++) {
        int s, e;
        cin >> s >> e;
        movies.push_back({s, e});
    }

    cout << helper() << endl;
}
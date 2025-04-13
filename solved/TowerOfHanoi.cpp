#include <bits/stdc++.h>
using namespace std;

void hanoi(int src, int dest, int mid, int n) {
    if (n == 1) {
        cout << src << " " << dest << "\n";
    } else {
        hanoi(src, mid, dest, n - 1);
        cout << src << " " << dest << "\n";
        hanoi(mid, dest, src, n - 1);

    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int n;
    cin >> n;
    cout << (1 << n) - 1 << "\n";
    hanoi(1, 3, 2, n);
}
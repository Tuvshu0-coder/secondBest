#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;

    int lcmA = a[0];
    for (int i = 1; i < n; i++) lcmA = lcm(lcmA, a[i]);

    int gcdB = b[0];
    for (int i = 1; i < m; i++) gcdB = gcd(gcdB, b[i]);

    int count = 0;
    for (int x = lcmA; x <= gcdB; x += lcmA) {
        if (gcdB % x == 0) count++;
    }

    cout << count << endl;
}
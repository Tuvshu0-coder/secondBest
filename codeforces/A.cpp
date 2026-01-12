#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        string s;
        cin >> s;
        int cntY = 0;
        for (char c : s) cntY += (c == 'Y');
        cout << (cntY <= 1 ? "YES" : "NO") << '\n';
    }
    return 0;
}

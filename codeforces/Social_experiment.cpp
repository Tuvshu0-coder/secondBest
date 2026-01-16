#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int N;
    cin >> N;

    if (N == 2) {
        cout << 2 << '\n';
        return;
    }
    if (N == 3) {
        cout << 3 << '\n';
        return;
    }

    if (N % 2 == 0) cout << 0 << '\n';
    else cout << 1 << '\n';
}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
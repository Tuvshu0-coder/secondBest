#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int s, k, m;
    cin >> s >> k >> m;
    int flips = m / k;
    int delta = m % k;

    int r;
    if (s <= k) {
        r = s;
    } else {
        r = (flips % 2 == 0) ? s : k;
    }

    int ans = r - delta;
    if (ans < 0) ans = 0;
    cout << ans << '\n';
}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
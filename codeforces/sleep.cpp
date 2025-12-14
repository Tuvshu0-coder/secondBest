#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int n, k;
    string binary;
    cin >> n >> k >> binary;

    int sleep = 0;
    int blocked_until = -1;

    for (int i = 0; i < n; i++) {
        if (binary[i] == '1') {
            blocked_until = i + k;
        } else { 
            if (i > blocked_until) {
                sleep++;            
            }
        }
    }

    cout << sleep << "\n";
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--)
        solve();
}

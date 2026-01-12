#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int a, b;
    cin >> a >> b;

    // k ≡ 9 * (b - a) (mod 17)
    int k = (9 * (b - a)) % 17;
    if (k < 0) k += 17;

    int N = 100 * k + b;

    cout << N << endl;
}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
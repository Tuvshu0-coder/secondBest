#include <bits/stdc++.h>
using namespace std;
#define int long long

int gcd(int a, int b) {
    int res = min(a, b);
    while (res > 1) {
        if (a % res == 0 && b % res == 0)
            break;
        res--;
    }
	return res;
}

void solve(){
    int a, b, c;
    cin >> a >> b >> c;
    int pre, suff;
    pre = abs(a-b);
    suff = abs(b-c);
    cout << gcd(pre, suff);
}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
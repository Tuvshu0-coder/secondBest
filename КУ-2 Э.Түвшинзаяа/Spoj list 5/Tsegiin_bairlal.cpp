#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int x, y;
    cin >> x >> y;
    if(x>0 && y>0) cout << "I";
    if(x<0 && y>0) cout << "II";
    if(x<0 && y<0) cout << "III";
    if(x>0 && y<0) cout << "IV";
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // int T;
    // cin >> T;
    // while(T--)
    solve();

}
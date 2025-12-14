#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int a, b, size;
    string ans;
    cin >> a >> b;
    size = log10(a)+1;
    for(int i=0; i<size; i++){
        if(a%10 != b%10){
            ans += "1";
        }else{
            ans += "0";
        }
        a/=10;
        b/=10;
    }
    reverse(ans.begin(), ans.end());
    cout << ans;
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // int T;
    // cin >> T;
    // while(T--)
    solve();

}
#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int N, K;
    cin >> N >> K;

    vector<int> dp(N+1, 0);
    dp[0] = 1;

    for(int i=1; i<=N; i++){
        for(int s = 1; s<=i; s++){
            dp[s] = dp[s] + dp[s-i];
        }
    }
    cout << dp[N]%K;

}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
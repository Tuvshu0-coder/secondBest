#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int Y, X;
    cin >> Y >> X;
    const int INF = 1e9;
    vector<vector<int>> cost(Y, vector<int>(X, 0));
    vector<vector<int>> dp(Y,vector<int>(X, INF));

    for(int i=0; i<Y; i++){
        for(int j=0; j<X; j++){
            cin >> cost[i][j];  // inputing the tulbur
        }
    }

    dp[0][0] = cost[0][0]; // starting point

    for(int i=0; i<Y; i++){
        for(int j=0; j<X; j++){
            if(i>0)
                dp[i][j] = min(dp[i][j], dp[i-1][j] + cost[i][j]); // y
            if(j>0)
                dp[i][j] = min(dp[i][j], dp[i][j-1] + cost[i][j]); // x
        }                                                              // if both bigger than zero it will choose the smaller 
    }
    cout << dp[Y-1][X-1] << '\n'; // after the final loops the smallest cost will be found.



}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
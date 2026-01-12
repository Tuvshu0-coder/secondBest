#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    int N;
    cin >> N;
    vector<vector<int>> dp(N + 1, vector<int> (N+1, 0));
    vector<vector<int>> a(N+1, vector<int> (N+1, 0));
    for(int i=1; i<=N; i++){
        for(int j=1; j<=i; j++){
            cin >> a[i][j];
        }
    }

    dp[1][1] = a[1][1];

    for(int i=2; i<=N; i++){
        for(int j=1; j<=i; j++ ){
            if(j==1){
                dp[i][j] = dp[i-1][1] + a[i][j];
            }else if(j == i){
                dp[i][j] = dp[i-1][j-1] + a[i][j];
            }else{
                dp[i][j] = max(dp[i-1][j-1], dp[i-1][j]) + a[i][j];
            }
        }
    }

    int answer = 0;

    for(int i=1; i<=N; i++){
        answer = max(answer, dp[N][i]);
    }
    cout << answer << '\n';

}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
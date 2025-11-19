#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, target, count=0;
    cin >> n >> target;
    
    if(n >= target){
        cout << n - target << '\n';
        return 0;
    }
    // while(target > n){
    //     if(target%2==0){
    //         target= target/2;
    //     }else{
    //         target++;
    //     }
    //     count++;


    // }

    // cout << count+n-target;
    vector<int> dp(target+1, INT_MAX);
    dp[n] = 0;
    
    for(int i = n; i <= target; i++){
        if(dp[i] == INT_MAX) continue;

        
        if(i*2 <= target){
            dp[i*2] = min(dp[i*2], dp[i] + 1);
        }
        if(i > 1){
            dp[i-1] = min(dp[i-1], dp[i] + 1);
        }
        
    }
    
    cout << dp[target] << '\n';
    return 0;
}
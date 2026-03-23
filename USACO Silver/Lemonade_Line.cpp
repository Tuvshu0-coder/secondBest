#include <bits/stdc++.h>
using namespace std;
#define int long long

bool isGreater(int a, int b){
    return a > b;
}

void solve(){
    freopen("lemonade.in", "r", stdin);
    freopen("lemonade.out", "w", stdout);
    int N;
    cin >> N;
    vector<int> arr(N);
    for(int &t : arr) cin >> t;
    int minCow = 0;
    sort(arr.begin(), arr.end() , isGreater);
    for(int i=0; i<N; i++){
        if(i <= arr[i]) minCow ++;
        else{
            break;
        }
    }
    cout << minCow;
    
    
    
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
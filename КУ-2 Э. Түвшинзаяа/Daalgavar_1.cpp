#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    vector<int> arr = {1, 1, 2, 3, 5, 8, 13};
    int N;
    cin >> N;
    for(int i=0; i<N; i++){
        cout << arr[ i % arr.size()] << ' ';
    }
}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
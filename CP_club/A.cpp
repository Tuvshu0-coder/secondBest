#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#define int long long

void solve(){
    int N;
    cin >> N;
    bool isSame = false;
    vector<int> arr(N);
    for(int i=0; i<N; i++){
        cin >> arr[i];
    }
    sort(arr.begin(), arr.end());
    for(int i=1; i<N; i++){
        if(arr[i] == arr[i-1]){
            cout << "NO" << '\n';
            isSame = true;
            break;
        }
    }
    if(!isSame){
        cout << "YES" << '\n';
    }
}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
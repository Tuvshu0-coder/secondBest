// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define int long long

void solve(){
    int N;
    vector<int> arr(N);
    for(int i=0; i<N; i++) cin >> arr[i];

    for(int i=0; i<N; i++){
        int greater=0, smaller=0;
        for(int j=i+1; j<N; j++){
            if(arr[j] > arr[i]) greater++;
            else if(arr[j] < arr[i] ) smaller++;
        }
        cout << max(greater, smaller) << " ";
    }
    cout << "\n";

    // int maximum = arr[N-1];
    // for(int i=N-1; i>=0; i--){
    //     maximum = max(maximum, arr[i]);
    //     suffMax[i] = maximum;
    // }


}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
//#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
#define int long long

void solve(){
    int N, sum=0;
    cin >> N;
    vector<int> arr(N);
    unordered_map<int, int> um;
    
    for(int i=0; i<N; i++){
        cin >> arr[i];
    }
    for(int i=N-1; i>=0; i--){
        if(um[arr[i]] ==0){
            um[arr[i]]++;
            sum++;
        }else{
            break;
        }
    }
    cout << N - sum << '\n';
}

signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
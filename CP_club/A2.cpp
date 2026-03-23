//#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
#define int long long

void solve(){
    int S, i=9;
    cin >> S;
    // string res;
    

    
    vector<int> res;
    while(S > 0 && i > 0){
        if(S >= i){
            res.push_back(i);
            // cout << i;
            S -= i;
        }
        i--;
    }

    for(int i=res.size()-1; i>=0; i--){
        cout << res[i];
    }
    cout << '\n';
}



signed main(){
    int T;
    cin >> T;
    while(T--)
    solve();
}
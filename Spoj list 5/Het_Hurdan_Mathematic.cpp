#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    string first, second;
    cin >> first >> second;
    for(int i=0; i<first.size(); i++){
        if(first[i] == second[i]){
            cout << 0;
        }else{
            cout << 1;
        }
    }



}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}
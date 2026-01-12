#include <bits/stdc++.h>
using namespace std;
#define int long long

void solve(){
    // char shit;
    // cin >> shit;
    // char poop = shit + 1;
    // cout << poop;
    string idree;
    cin >> idree;

    int result = 0;
    for(char c : idree){
        result = result*26 + (c - 'A' + 1);
    }
    cout << result;


}

signed main(){
    // int T;
    // cin >> T;
    // while(T--)
    solve();
}